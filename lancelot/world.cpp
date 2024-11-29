#include "world.h"
#include <iostream>


namespace lancelot{

    
        World::World(unsigned maxContacts, unsigned iterations){
            std::vector<Particle*>* PR_ptr = &ParticleRegistry;
            this->Detector = ParticleContactDetector(PR_ptr, maxContacts,80.0,Vector3d(0, 0, 0));
            this->Resolver = ParticleContactsResolver(iterations);
        }

    

        // Remove all forces from the accumulator.
        void World::startFrame(){
            std::vector<Particle*>::iterator i = ParticleRegistry.begin();
            for (; i != ParticleRegistry.end(); i++){
                (*i)->clearAccumulator();
            }
        }
            

        void World::integrate(real duration){
           std::vector<Particle*>::iterator i = ParticleRegistry.begin();
            for (; i != ParticleRegistry.end(); i++){
                (*i)->integrate(duration);
            }
        }

        int World::printStuff(){
            std::vector<ParticleContact*>& contactRegistry = Detector.getContactRegistry();
            if (contactRegistry.empty()) {
                return 0;
            }
            std::vector<ParticleContact*>::iterator i = contactRegistry.begin();
            for (; i != contactRegistry.end(); i++){
                if((*i)->particle2){
                }else
                {std::cout<<"particle 2 is NULL"<<std::endl;};
            }
            return 1;
        }



        void World::simulatePhysics(real duration){
            /* calculating the force accumulated using force generators */
            PForceRegistry.updateForces(duration);
            
            /* integrating the forces and updating the position of the particles */
            integrate(duration);

            /* finding and resolving contacts */
            Detector.GenerateContacts();
            Detector.GenerateWallContacts();
            
            

            Resolver.resolveContacts(Detector.getContactRegistry(),duration);

        }
        

    };
