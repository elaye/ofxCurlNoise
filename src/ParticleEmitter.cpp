#include "ParticleEmitter.h"

uint ParticleEmitter::count = 0;

// void ParticleEmitter::setup(int n){
void ParticleEmitter::setup(){
	// particleNumber = n;

	parameters.setName("Particle emitter " + ofToString(id+1));
	parameters.add(averageLifespan.set("Average lifespan", 120.0, 1.0, 600.0));
	parameters.add(lifespanVariation.set("Lifespan variation", 50.0, 0.0, 100.0));
	parameters.add(emissionAngle.set("Emission angle", M_PI/6.0, 0.0, M_PI));
	parameters.add(averageVelocity.set("Average vel.", 0.5, 0.0, 10.0));
	parameters.add(velocityVariation.set("Vel. variation", 100.0, 0.0, 100.0));
	parameters.add(bUseEmitterVelocity.set("Use emitter vel.", true));
	parameters.add(bUseEmitterVelDir.set("Emitter vel. dir.", true));
	parameters.add(velocityScale.set("Emitter Vel. scale", 1.0, -10.0, 10.0));
	parameters.add(radius.set("Radius", 3.0, 1.0, 30.0));

	prevPos = ofVec3f(0);
	velDir = ofVec3f(0);
	// emissionDir = ofVec3f(0, 1, 0);
	orientation.makeRotate(ofVec3f(0, 0, 1), ofVec3f(0, -1, 0));
	
	prevTime = 0.0;

	pos = ofVec3f(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0);
	vel = ofVec3f(0, 0, 0);
	acc = ofVec3f(0, 0, 0);
	loadShader();	
}

void ParticleEmitter::update(float x, float y, float z){
	prevPos = pos;
	float dt = ofGetElapsedTimef() - prevTime;
	ofPoint newVel = (pos - ofPoint(x, y, z))*dt;
	pos = ofPoint(x, y, z);
	acc = (vel - newVel)*dt;
	vel = newVel;
	prevTime = ofGetElapsedTimef();
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	shader.begin();
		shader.setUniform4f("pos", pos.x, pos.y, pos.z, 1.0);
		shader.setUniform4f("vel", vel.x, vel.y, vel.z, 1.0);
		shader.setUniform4f("acc", acc.x, acc.y, acc.z, 1.0);
		
		ofVec4f rot = orientation.asVec4();
		shader.setUniform4f("prevPos", prevPos.x, prevPos.y, prevPos.z, 1.0);
		// shader.setUniform3f("emissionDir", emissionDir.x, emissionDir.y, emissionDir.z);
		shader.setUniform4f("orientation", rot.x, rot.y, rot.z, rot.w);
		shader.setUniform1f("radius", radius);
		shader.setUniform1f("averageLifespan", averageLifespan);
		shader.setUniform1f("lifespanVariation", lifespanVariation);
		shader.setUniform1f("emissionAngle", emissionAngle);
		shader.setUniform1f("averageVelocity", averageVelocity);
		shader.setUniform1f("velocityScale", velocityScale);
		shader.setUniform1f("velocityVariation", velocityVariation);
		shader.setUniform1f("useEmitterVelocity", (bUseEmitterVelocity)? 1.0 : -1.0);
		shader.setUniform1i("id", id);
		shader.setUniform1i("count", count);

		// shader.dispatchCompute(particleNumber/WGS, 1, 1);
		shader.dispatchCompute(ParticleManager::getParticleNumber()/WORK_GROUP_SIZE, 1, 1);
	shader.end();
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	ofVec3f velDir = 0.9*velDir + 0.1*vel;
	if(bUseEmitterVelDir && vel != ofVec3f(0.0)){
		orientation.makeRotate(ofVec3f(0, 0, 1), -velDir);
	}
}

void ParticleEmitter::loadShader(){
	ostringstream cs;
	cs << "#version 430" << endl;
	cs << "layout(local_size_x = " << WORK_GROUP_SIZE << ", local_size_y = 1, local_size_z = 1) in;" << endl;
	cs << STRINGIFY(

		struct Particle{
		    vec4 pos;
		    vec4 vel;
		    vec4 acc;
		};

		uniform vec4 pos;
		uniform vec4 vel;
		uniform vec4 acc;

		layout(std430, binding=0) buffer particles{
			Particle p[];
		};

		layout(std430, binding=1) buffer lifespans{
			float lifespan[];
		};

		layout(std430, binding=2) buffer emitterIds{
			int emitterId[];
		};
	) << endl;

	cs << getShaderCode() << endl;

	shader.setupShaderFromSource(GL_COMPUTE_SHADER, cs.str());
	shader.linkProgram();
}

string ParticleEmitter::getShaderCode(){
	ostringstream cs;
	cs << "#define M_PI 3.1415926535897932384626433832795" << endl;
	cs << STRINGIFY(

		uniform vec4 prevPos;
		// uniform vec3 emissionDir;
		uniform vec4 orientation;
		uniform float radius;
		uniform float velocityScale;
		uniform float averageLifespan;
		uniform float lifespanVariation;
		uniform float emissionAngle;
		uniform float averageVelocity;
		uniform float velocityVariation;
		uniform float useEmitterVelocity;
		uniform int id;
		uniform int count;
		
		uint rng_state;
	);

	// float rand(float x, float y)
	// vec3 rotate(vec3 v, vec4 q)
	cs << getUtilityCode();

	// void respawn(uint gid)
	cs << getEmitterCode();

	// void main()
	cs << getMainCode();

	return cs.str();		
}

string ParticleEmitter::getMainCode(){
	ostringstream s;
	s << STRINGIFY(
		void main(){
			uint gid = gl_GlobalInvocationID.x;
			rng_state = gid;

			// Dead?
			if(lifespan[gid] < 0.0){
				if(rng_state % count == id){
					respawn(gid);
					emitterId[gid] = int(id);
				}
			}
		}
	) << endl;
	return s.str();
}

string ParticleEmitter::getEmitterCode(){
	ostringstream s;
	s << STRINGIFY(
		// Random lifespan based on emitter's parameters
		float newLifespan(){
			float var = rand(-lifespanVariation, lifespanVariation)/100.0;
			return averageLifespan*(1.0 + var);
		}

		// Random position based on emitter's parameters
		vec4 newPosition(){
			float theta = rand(0.0, 2*M_PI);
			// Uniform random point on sphere
			float u = rand(-1.0, 1.0);
			float x = radius*sqrt(1.0-u*u)*cos(theta);
			float y = radius*sqrt(1.0-u*u)*sin(theta);
			float z = radius*u;
			// float z = 0.0;
			vec3 intPos = mix(pos.xyz, prevPos.xyz, rand(0.0, 1.0));
			vec3 newPos = intPos + vec3(x, y, z);
			return vec4(newPos, 1.0);
		}

		// Random velocity in a cone based on emitter's parameters
		vec4 newVelocity(){
			float phi = rand(0, 2*M_PI);
			float nz = rand(cos(emissionAngle), 1.0);
			float nx = sqrt(1-nz*nz)*cos(phi);
			float ny = sqrt(1-nz*nz)*sin(phi);
			vec3 velDir = rotate(vec3(nx, ny, nz), orientation);
			float velNorm = averageVelocity*(1.0 + rand(-velocityVariation, velocityVariation)/100.0);			
			vec3 newVel = velDir*velNorm;
			// newVel = velNorm*rotate(emissionDir, orientation);
			newVel = velNorm*rotate(vec3(nx, ny, nz), orientation);
			// newVel *= velocityScale;
			if(useEmitterVelocity > 0.0){
				newVel += vel.xyz*velocityScale;
			}
			return vec4(newVel.xyz, 1.0);
		}

		// Respawn particle p[gid] at a random emitter position
		void respawn(uint gid){
			lifespan[gid] = newLifespan();
			p[gid].pos = newPosition();
			p[gid].vel = newVelocity();
			p[gid].acc = vec4(0.0, 0.0, 0.0, 1.0);
		}
	) << endl;
	return s.str();
}

string ParticleEmitter::getUtilityCode(){
	ostringstream s;
	s << STRINGIFY(
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
			return v + 2.0 * cross( cross( v, q.xyz ) + q.w * v, q.xyz );			
		}
	) << endl;
	return s.str();
}

void ParticleEmitter::draw(){
	ofNoFill();
	ofDrawCircle(pos, radius);
}