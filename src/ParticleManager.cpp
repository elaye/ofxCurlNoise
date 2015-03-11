#include "ParticleManager.h"

int ParticleManager::particleNumber = 0;

void ParticleManager::setup(int n){
	particleNumber = n;
	particles.resize(n);

	initParticles();

	particlesBuffer.allocate(particles, GL_STATIC_DRAW);
	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	vector<float> lifespans(n);
	for(auto& l : lifespans){
		l = ofRandom(0.0, 120.0);
	}
	lifespanBuffer.allocate(lifespans, GL_STATIC_DRAW);
	lifespanBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);


	vector<int> emitterIds(n, 0);
	emitterIdBuffer.allocate(emitterIds, GL_STATIC_DRAW);
	emitterIdBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);

	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));

	loadShader();
}

void ParticleManager::setAttributes(ofShader& renderShader){
	vbo.setAttributeBuffer(renderShader.getAttributeLocation("lifespan"), lifespanBuffer, 1, 0);
	vbo.setAttributeBuffer(renderShader.getAttributeLocation("emitterId"), emitterIdBuffer, 1, 0);
}

void ParticleManager::initParticles(){
	for(uint i = 0; i < particles.size(); ++i){
		particles[i].pos = ofVec4f(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0, 1.0);
		particles[i].vel = ofVec4f(0.0);
		particles[i].acc = ofVec4f(0.0);
	}	
}

void ParticleManager::update(){
	shader.begin();
		shader.dispatchCompute(particles.size()/WORK_GROUP_SIZE, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	shader.end();

	// Particle* p = particlesBuffer.map<Particle>(GL_READ_ONLY);
	// 	ofLog() << p[100].vel;
	// particlesBuffer.unmap();
}

void ParticleManager::draw(){
	vbo.draw(GL_POINTS, 0, vbo.getNumVertices());	
}

void ParticleManager::loadShader(){
	ostringstream cs;
	cs << "#version 430" << endl;
	cs << "layout(local_size_x = " << WORK_GROUP_SIZE << ", local_size_y = 1, local_size_z = 1) in;" << endl;
	cs << STRINGIFY(

		struct Particle{
			vec4 pos;
			vec4 vel;
			vec4 acc;
		};

		layout(std430, binding=0) buffer particles{
			Particle p[];
		};

		layout(std430, binding=1) buffer lifespans{
			float lifespan[];
		};

		layout(std430, binding=2) buffer emitterIds{
			int emitterId[];
		};
		void main(){
			uint gid = gl_GlobalInvocationID.x;
			lifespan[gid] -= 1.0;
			
			p[gid].vel.xyz += p[gid].acc.xyz;
			p[gid].pos.xyz += p[gid].vel.xyz;
			p[gid].acc.xyz = vec3(0.0);
		}
	);
	shader.setupShaderFromSource(GL_COMPUTE_SHADER, cs.str());
	shader.linkProgram();
}