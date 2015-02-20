#include "ParticleEmitter.h"

void ParticleEmitter::setup(int n){
	pos = ofPoint(ofGetMouseX(), ofGetMouseY());
	vel = ofPoint(0, 0);
	acc = ofPoint(0, 0);

	particles.resize(n);
	initParticles();
	particlesBuffer.allocate(particles, GL_STATIC_DRAW);
	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));
	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	prevTime = 0.0;

	loadShader();
}

void ParticleEmitter::initParticles(){

	for(uint i = 0; i < particles.size(); ++i){
		particles[i].pos = ofVec4f(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0, 1.0);
		particles[i].vel = ofVec4f(0.0);
		particles[i].acc = ofVec4f(0.0);
		particles[i].lifespan.x = ofRandom(120.0);
	}	
}

void ParticleEmitter::update(float x, float y){
	float dt = ofGetElapsedTimef() - prevTime;
	ofPoint newVel = ofPoint(pos.x - x, pos.y - y)*dt;
	pos = ofPoint(x, y);
	acc.x = (vel.x - newVel.x)*dt;
	acc.y = (vel.y - newVel.y)*dt;
	vel = newVel;
	prevTime = ofGetElapsedTimef();

	// glMemoryBarrier(GL_ALL_BARRIER_BITS);
	shader.begin();
		shader.setUniform2f("emitterPos", pos.x, pos.y);
		shader.setUniform2f("emitterVel", vel.x, vel.y);
		shader.dispatchCompute(particles.size()/WORK_GROUP_SIZE, 1, 1);
	shader.end();
	// glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ParticleEmitter::loadShader(){
	ostringstream cs;
	cs << "#version 430\n";
	cs << "layout(local_size_x = " << WORK_GROUP_SIZE << ", local_size_y = 1, local_size_z = 1) in;\n";
	cs << STRINGIFY(
		struct Particle{
		    vec4 pos;
		    vec4 vel;
		    vec4 acc;
		    vec4 lifespan;
		};

		layout(std430, binding=0) buffer particles{
			Particle p[];
		};		

		uniform vec2 emitterPos;
		uniform vec2 emitterVel;

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

		void main(){
			uint gid = gl_GlobalInvocationID.x;
			rng_state = gid;

			p[gid].lifespan.x -= 1.0;
			if(p[gid].lifespan.x < 0.0){
				p[gid].pos = vec4(emitterPos, 0.0, 1.0);
				p[gid].vel = vec4(-emitterVel*rand(-2.0, 2.0)+vec2(rand(-0.5, 0.5), rand(-0.5, 0.5)), 0.0, 1.0);
			// 	p[gid].acc = vec4(0.0, 0.0, 0.0, 1.0);
				p[gid].lifespan.x = 120.0*rand(0.8, 1.2);
			}else{
				p[gid].vel.xyz += p[gid].acc.xyz;
				p[gid].pos.xyz += p[gid].vel.xyz;
			}
		}
	);

	shader.setupShaderFromSource(GL_COMPUTE_SHADER, cs.str());
	shader.linkProgram();	
}