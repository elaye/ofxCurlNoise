#include "ParticleManager.h"

void ParticleManager::setup(int k, int n){
	emittersNb = k;
	particles.resize(n);
	initParticles();
	particlesBuffer.allocate(particles, GL_STATIC_DRAW);
	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	
	// emittersBuffer.allocate(vector<Emitter>(begin(emitters), end(emitters)), GL_DYNAMIC_DRAW);
	// emittersBuffer.allocate(getEmittersData(), GL_DYNAMIC_DRAW);
	emittersBuffer.allocate(emittersNb*sizeof(Emitter), GL_DYNAMIC_DRAW);
	// vector<Emitter> emitters = {ParticleEmitter().getData(), ParticleEmitter().getData()};
	// emittersBuffer.allocate(emitters, GL_DYNAMIC_DRAW);
	emittersBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));

	// ofLog() << sizeof(Emitter);

	loadShader();	
}

// void ParticleManager::addEmitter(ParticleEmitter* emitter){
// 	emitters.push_back(emitter);
// }

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
		// shader.setUniform2f("emitterPrevPos", prevPos.x, prevPos.y);
		// shader.setUniform2f("emitterPos", pos.x, pos.y);
		// shader.setUniform2f("emitterVel", vel.x, vel.y);
		// shader.setUniform1f("emitterRadius", emitterRadius);
		// shader.setUniform1f("averageLifespan", averageLifespan);
		// shader.setUniform1f("lifespanVariation", lifespanVariation);
		// shader.setUniform1f("averageVelocity", averageVelocity);
		// shader.setUniform1f("velocityVariation", velocityVariation);
		// shader.setUniform1f("useEmitterVelocity", (bUseEmitterVelocity)? 1.0 : -1.0);
		// shader.setUniform1f("velocityScale", velocityScale);
		shader.setUniform1i("emittersNb", emittersNb);
		shader.dispatchCompute(particles.size()/WORK_GROUP_SIZE, 1, 1);
	shader.end();	
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ParticleManager::updateEmitter(ParticleEmitter& emitter){
	// ofLog() << emitter.getId();
	// Emitter data[1] = {emitter.getData()};
	// ofLog() << data[0].pos;
	// emittersBuffer.updateData(emitter.getId()*sizeof(Emitter), sizeof(Emitter), data);
	vector<Emitter> data = {emitter.getData()};
	// ofLog() << emitter.getId()*sizeof(Emitter);
	emittersBuffer.updateData(emitter.getId()*sizeof(Emitter), data);

	// Emitter* em = emittersBuffer.map<Emitter>(GL_READ_ONLY);
	// 	ofLog() << (em->pos == data[0].pos);
	// emittersBuffer.unmap();
}

void ParticleManager::loadShader(){
	ostringstream cs;
	cs << "#version 430" << endl;
	cs << "#define M_PI 3.1415926535897932384626433832795" << endl;
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
			// float radius;
			// float velocityScale;
			// float averageLifespan;
			// float lifespanVariation;
			// float averageVelocity;
			// float velocityVariation;
			// float useEmitterVelocity;
			vec4 lifespan;
			vec4 velParam;
			vec4 radius;
		};

		layout(std430, binding=0) buffer particles{
			Particle p[];
		};

		layout(std430, binding=1) buffer emitters{
			Emitter e[];
		};		

		uniform int emittersNb;

		// uniform vec2 emitterPrevPos;
		// uniform vec2 emitterPos;
		// uniform vec2 emitterVel;
		// uniform float emitterRadius;
		// uniform float averageLifespan;
		// uniform float lifespanVariation;
		// uniform float averageVelocity;
		// uniform float velocityVariation;
		// uniform float useEmitterVelocity;
		// uniform float velocityScale;

		uint rng_state;

		float rand_xorshift(){
			// Xorshift algorithm from George Marsaglia's paper
			rng_state ^= (rng_state << 13);
			rng_state ^= (rng_state >> 17);
			rng_state ^= (rng_state << 5);
			float r = float(rng_state) / 4294967296.0;
			return r;
		}

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

		void respawn(uint gid){
			uint id = rng_state % emittersNb;
			// id = 1;
			Emitter em = e[id];
			// em = e[0];
			float theta = rand(0.0, 2*M_PI);
			float r = rand(0.0, em.radius.x);
			float x = r*cos(theta);
			float y = r*sin(theta);
			vec2 newPos = mix(em.pos.xy, em.prevPos.xy, rand(0.0, 1.0));
			// vec2 newPos = mix(e[1].pos.xy, em.prevPos.xy, rand(0.0, 1.0));
			p[gid].pos = vec4(newPos+vec2(x, y), 0.0, 1.0);
			float velNorm = em.velParam.y*(1.0 + rand(-em.velParam.z, em.velParam.z)/100.0);
			float vx = velNorm*cos(theta);
			float vy = velNorm*sin(theta);
			if(em.velParam.w > 0.0){
				p[gid].vel = vec4(em.vel.xy*em.velParam.x + vec2(vx, vy), 0.0, 1.0);
			}
			else{
				p[gid].vel = vec4(-normalize(em.vel.xy)*vec2(vx, vy), 0.0, 1.0);
			}
			p[gid].acc = vec4(0.0, 0.0, 0.0, 1.0);
			p[gid].lifespan.x = em.lifespan.x*(1.0 + rand(-em.lifespan.y, em.lifespan.y)/100.0);
		}

		void main(){
			uint gid = gl_GlobalInvocationID.x;
			// rng_state = uint(floor(p[gid].pos.x*p[gid].pos.y*1000))*gid;
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