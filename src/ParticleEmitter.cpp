#include "ParticleEmitter.h"

void ParticleEmitter::setup(int n){
	pos = ofPoint(ofGetMouseX(), ofGetMouseY());
	prevPos = pos;
	vel = ofPoint(0, 0);
	acc = ofPoint(0, 0);

	particles.resize(n);
	initParticles();
	particlesBuffer.allocate(particles, GL_STATIC_DRAW);
	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));
	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	prevTime = 0.0;

	parameters.setName("Particle emitter");
	parameters.add(averageLifespan.set("Average lifespan", 120.0, 1.0, 3600.0));
	parameters.add(lifespanVariation.set("Lifespan variation", 100.0, 0.0, 100.0));
	parameters.add(averageVelocity.set("Average vel.", 0.5, 0.0, 2.5));
	parameters.add(velocityVariation.set("Vel. variation", 100.0, 0.0, 100.0));
	parameters.add(bAddEmitterVelocity.set("Add emitter vel.", true));
	parameters.add(emitterRadius.set("Radius", 3.0, 1.0, 30.0));

	loadShader();
}

void ParticleEmitter::initParticles(){

	for(uint i = 0; i < particles.size(); ++i){
		particles[i].pos = ofVec4f(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0, 1.0);
		particles[i].vel = ofVec4f(0.0);
		particles[i].acc = ofVec4f(0.0);
		particles[i].lifespan.x = averageLifespan*(1.0 + ofRandom(-lifespanVariation, lifespanVariation)/100.0);
	}	
}

void ParticleEmitter::update(float x, float y){
	float dt = ofGetElapsedTimef() - prevTime;
	ofPoint newVel = ofPoint(pos.x - x, pos.y - y)*dt;
	prevPos = pos;
	pos = ofPoint(x, y);
	acc.x = (vel.x - newVel.x)*dt;
	acc.y = (vel.y - newVel.y)*dt;
	vel = newVel;
	prevTime = ofGetElapsedTimef();

	shader.begin();
		shader.setUniform2f("emitterPrevPos", prevPos.x, prevPos.y);
		shader.setUniform2f("emitterPos", pos.x, pos.y);
		shader.setUniform2f("emitterVel", vel.x, vel.y);
		shader.setUniform1f("emitterRadius", emitterRadius);
		shader.setUniform1f("averageLifespan", averageLifespan);
		shader.setUniform1f("lifespanVariation", lifespanVariation);
		shader.setUniform1f("averageVelocity", averageVelocity);
		shader.setUniform1f("velocityVariation", velocityVariation);
		shader.setUniform1f("addEmitterVelocity", (bAddEmitterVelocity)? -1.0 : 1.0);
		shader.dispatchCompute(particles.size()/WORK_GROUP_SIZE, 1, 1);
	shader.end();
}

void ParticleEmitter::loadShader(){
	ostringstream cs;
	cs << "#version 430\n";
	cs << "#define M_PI 3.1415926535897932384626433832795\n";
	cs << "layout(local_size_x = " << WORK_GROUP_SIZE << ", local_size_y = 1, local_size_z = 1) in;\n";
	cs << STRINGIFY(
	    precision highp float;

		struct Particle{
		    vec4 pos;
		    vec4 vel;
		    vec4 acc;
		    vec4 lifespan;
		};

		layout(std430, binding=0) buffer particles{
			Particle p[];
		};		

		uniform vec2 emitterPrevPos;
		uniform vec2 emitterPos;
		uniform vec2 emitterVel;
		uniform float emitterRadius;
		uniform float averageLifespan;
		uniform float lifespanVariation;
		uniform float averageVelocity;
		uniform float velocityVariation;
		uniform float addEmitterVelocity;

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
			float theta = rand(0.0, 2*M_PI);
			float r = rand(0.0, emitterRadius);
			float x = r*cos(theta);
			float y = r*sin(theta);
			vec2 newPos = mix(emitterPos, emitterPrevPos, rand(0.0, 1.0));
			p[gid].pos = vec4(newPos+vec2(x, y), 0.0, 1.0);
			float velNorm = averageVelocity*(1.0 + rand(-velocityVariation, velocityVariation)/100.0);
			float vx = velNorm*cos(theta);
			float vy = velNorm*sin(theta);
			if(addEmitterVelocity > 0.0){
				p[gid].vel = vec4(-emitterVel + vec2(vx, vy), 0.0, 1.0);				
			}
			else{
				p[gid].vel = vec4(-normalize(emitterVel)*vec2(vx, vy), 0.0, 1.0);
			}
			p[gid].acc = vec4(0.0, 0.0, 0.0, 1.0);
			p[gid].lifespan.x = averageLifespan*(1.0 + rand(-lifespanVariation, lifespanVariation)/100.0);
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