#include "ParticleManager.h"

void ParticleManager::setup(int k, int n){
	emittersData.resize(k);
	particles.resize(n);

	initParticles();

	particlesBuffer.allocate(particles, GL_STATIC_DRAW);
	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));

	loadShader();	
}

void ParticleManager::initParticles(){
	for(uint i = 0; i < particles.size(); ++i){
		particles[i].pos = ofVec4f(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0, 1.0);
		particles[i].vel = ofVec4f(0.0);
		particles[i].acc = ofVec4f(0.0);
		particles[i].lifespan.x = ofRandom(0, 120.0);
		// particles[i].lifespan.x = 0.0;
	}	
}

void ParticleManager::update(){
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	shader.begin();
		for(uint i = 0; i < emittersData.size(); ++i){
			string is = ofToString(i);
			EmitterData& e(emittersData[i]);
			ofVec4f rot = e.velRotFromZ.asVec4();
			shader.setUniform4f("e["+is+"].pos", e.pos.x, e.pos.y, e.pos.z, e.pos.w);
			shader.setUniform4f("e["+is+"].vel", e.vel.x, e.vel.y, e.vel.z, e.vel.w);
			shader.setUniform4f("e["+is+"].acc", e.acc.x, e.acc.y, e.acc.z, e.acc.w);
			shader.setUniform4f("e["+is+"].prevPos", e.prevPos.x, e.prevPos.y, e.prevPos.z, e.prevPos.w);
			shader.setUniform4f("e["+is+"].velRotFromZ", rot.x, rot.y, rot.z, rot.w);
			shader.setUniform1f("e["+is+"].radius", e.radius);
			shader.setUniform1f("e["+is+"].averageLifespan", e.averageLifespan);
			shader.setUniform1f("e["+is+"].lifespanVariation", e.lifespanVariation);
			shader.setUniform1f("e["+is+"].velocityAngle", e.velocityAngle);
			shader.setUniform1f("e["+is+"].averageVelocity", e.averageVelocity);
			shader.setUniform1f("e["+is+"].velocityScale", e.velocityScale);
			shader.setUniform1f("e["+is+"].velocityVariation", e.velocityVariation);
			shader.setUniform1f("e["+is+"].useEmitterVelocity", (e.bUseEmitterVelocity)? 1.0 : -1.0);
			shader.setUniform1i("e["+is+"].id", e.id);
		}
		shader.dispatchCompute(particles.size()/WORK_GROUP_SIZE, 1, 1);
	shader.end();	
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ParticleManager::updateEmitter(EmitterData& emitterData){
	emittersData[emitterData.id] = emitterData;
}

void ParticleManager::loadShader(){
	ostringstream cs;
	cs << "#version 430" << endl;
	cs << "#define M_PI 3.1415926535897932384626433832795" << endl;
	cs << "#define EMITTERS_NB " << emittersData.size() << endl;
	cs << "layout(local_size_x = " << WORK_GROUP_SIZE << ", local_size_y = 1, local_size_z = 1) in;" << endl;
	cs << STRINGIFY(

		struct Particle{
		    vec4 pos;
		    vec4 vel;
		    vec4 acc;
		    vec4 lifespan;
		};

		struct Emitter{
			vec4 pos;
			vec4 vel;
			vec4 acc;
			vec4 prevPos;
			vec4 velRotFromZ;
			float radius;
			float velocityScale;
			float averageLifespan;
			float lifespanVariation;
			float velocityAngle;
			float averageVelocity;
			float velocityVariation;
			float useEmitterVelocity;
			int id;
		};

		layout(std430, binding=0) buffer particles{
			Particle p[];
		};
		
		uniform Emitter e[EMITTERS_NB];

		uint rng_state;

		// Random float number between 0.0 and 1.0
		float rand_xorshift(){
			// Xorshift algorithm from George Marsaglia's paper
			rng_state ^= (rng_state << 13);
			rng_state ^= (rng_state >> 17);
			rng_state ^= (rng_state << 5);
			float r = float(rng_state) / 4294967296.0;
			return r;
		}

		// Random number between min(x, y) and max(x, y)
		float rand(float x, float y){
			float high = 0;
			float low = 0;
			float randNum = 0;
			if (x == y) return x;
			high = max(x,y);
			low = min(x,y);
			randNum = low + (high-low) * rand_xorshift();
			return randNum;
		}

		// Rotate a vec3 using a quaternion
		vec3 rotate(vec3 v, vec4 q){
			// vec3 temp = cross(q.xyz, v) + q.w * v;
			// return (cross(temp, -q.xyz) + dot(q.xyz,v) * q.xyz + q.w * temp);
			return v + 2.0 * cross( cross( v, q.xyz ) + q.w * v, q.xyz );			
		}

		// Random lifespan based on emitter's parameters
		float newLifespan(Emitter em){
			float var = rand(-em.lifespanVariation, em.lifespanVariation)/100.0;
			return em.averageLifespan*(1.0 + var);
		}

		// Random position based on emitter's parameters
		vec4 newPosition(Emitter em){
			float theta = rand(0.0, 2*M_PI);
			// float r = rand(0.0, em.radius);
			// float x = r*cos(theta);
			// float y = r*sin(theta);
			// Uniform random point on sphere
			float u = rand(-1.0, 1.0);
			float x = em.radius*sqrt(1.0-u*u)*cos(theta);
			float y = em.radius*sqrt(1.0-u*u)*sin(theta);
			float z = em.radius*u;
			// float z = 0.0;
			vec3 intPos = mix(em.pos.xyz, em.prevPos.xyz, rand(0.0, 1.0));
			vec3 newPos = intPos + vec3(x, y, z);
			// vec2 newPos = mix(e[1].pos.xy, em.prevPos.xy, rand(0.0, 1.0));
			return vec4(newPos, 1.0);
		}

		// Random velocity based on emitter's parameters
		// vec4 newVelocity(Emitter em){
		// 	float theta = rand(0.0, 2*M_PI);
		// 	float velNorm = em.averageVelocity*(1.0 + rand(-em.velocityVariation, em.velocityVariation)/100.0);
		// 	float vx = velNorm*cos(theta);
		// 	float vy = velNorm*sin(theta);
		// 	vec3 newVel;
		// 	if(em.useEmitterVelocity > 0.0){
		// 		newVel.xy = em.vel.xy*em.velocityScale + vec2(vx, vy);
		// 	}
		// 	else{
		// 		newVel.xy = -normalize(em.vel.xy) * vec2(vx, vy);
		// 	}
		// 	return vec4(newVel.xy, 0.0, 1.0);
		// }

		// Random velocity in a cone based on emitter's parameters
		vec4 newVelocity(Emitter em){
			float phi = rand(0, 2*M_PI);
			float nz = rand(cos(em.velocityAngle), 1.0);
			float nx = sqrt(1-nz*nz)*cos(phi);
			float ny = sqrt(1-nz*nz)*sin(phi);
			vec3 velDir = rotate(vec3(nx, ny, nz), em.velRotFromZ);
			float velNorm = em.averageVelocity*(1.0 + rand(-em.velocityVariation, em.velocityVariation)/100.0);			
			vec3 newVel = velDir*velNorm;
			// newVel *= em.velocityScale;
			if(em.useEmitterVelocity > 0.0){
				newVel += em.vel.xyz*em.velocityScale;
			}
			return vec4(newVel.xyz, 1.0);
		}

		// Respawn particle p[gid] at a random emitter position
		void respawn(uint gid){
			uint id = rng_state % EMITTERS_NB;
			Emitter em = e[id];

			p[gid].lifespan.x = newLifespan(em);
			p[gid].pos = newPosition(em);
			// p[gid].pos = vec4(0.0, 0.0, 0.0, 1.0);
			p[gid].vel = newVelocity(em);
			p[gid].acc = vec4(0.0, 0.0, 0.0, 1.0);
		}

		void main(){
			uint gid = gl_GlobalInvocationID.x;
			rng_state = gid;

			p[gid].lifespan.x -= 1.0;
			// Dead?
			if(p[gid].lifespan.x < 0.0){
				respawn(gid);
			}
			else{
				p[gid].vel.xyz += p[gid].acc.xyz;
				p[gid].pos.xyz += p[gid].vel.xyz;
			}
		}
	);

	shader.setupShaderFromSource(GL_COMPUTE_SHADER, cs.str());
	shader.linkProgram();	
}