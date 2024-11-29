#include "pcontacts.h"
#include <iostream>
#include <queue>
#include <vector>

namespace lancelot{


        /* creates a particle contact record object*/
        ParticleContact::ParticleContact(Particle* particle1, Particle* particle2, real restitution,real penetration){
            this->particle1 = particle1;
            this->particle2 = particle2;
            this->restitution = restitution;
            this->penetration = penetration;
            this->contactNormal = (particle1->getPosition() - particle2->getPosition());
            this->contactNormal.normalize();
            this->isResolved = false;
            this->precomputedSeparatingVelocity = 0;
            this->isP2Immovable = false;
        }

        /** creates a particle contact record object
         * used when creating contacts with walls and other immovable obvjects the bool isP2Immovable is set to true
        */

        ParticleContact::ParticleContact(Particle* particle1, Particle* particle2, real restitution,real penetration, Vector3d contactNormal){
            this->particle1 = particle1;
            this->particle2 = particle2;
            this->restitution = restitution;
            this->penetration = penetration;
            this->contactNormal = contactNormal;
            this->isResolved = false;
            this->precomputedSeparatingVelocity = 0;
            this->isP2Immovable = true;
        }

       
        void ParticleContact::resolve(real duration){
        
            resolveVelocity(duration);
            resolveInterpenetration(duration);

        }


        /**
        * Calculates the separating velocity at this contact.
        */
        real ParticleContact::calculateSeparatingVelocity() const{
            Vector3d relativeVelocity = particle1->getVelocity();

            if(particle2) {
                relativeVelocity -= particle2->getVelocity();
            }

            return relativeVelocity.dotProduct(contactNormal);
        }

        void ParticleContact::resolveVelocity(real duration){

        
            real separatingVelocity = calculateSeparatingVelocity();

            // Check whether it needs to be resolved.
            if (separatingVelocity > 0){
            // The contact is either separating or stationary - there’s
            // no impulse required.
            return;
            }

            // Calculate the new separating velocity.
            real newSepVelocity = -separatingVelocity * restitution;

            // Check the velocity build-up due to acceleration only.
            Vector3d accCausedVelocity = particle1->getAcceleration();
            if (particle2) accCausedVelocity -= particle2->getAcceleration();

            real accCausedSepVelocity = accCausedVelocity * contactNormal * duration;

            // If we’ve got a closing velocity due to acceleration build-up,
            // remove it from the new separating velocity.

            if (accCausedSepVelocity < 0)
            {
            newSepVelocity += restitution * accCausedSepVelocity;
            // Make sure we haven’t removed more than was
            // there to remove.
            if (newSepVelocity < 0) newSepVelocity = 0;
            }

            

            // calulate the change in velocitiy.
            real deltaVelocity = newSepVelocity - separatingVelocity;


            real totalInvMass = particle1->getInverseMass();
            if(particle2){
                totalInvMass += particle2->getInverseMass();
            }

            // If all particles have infinite mass, then impulses have no effect.
            if (totalInvMass <= 0){
                return;
            }

            //calculating teh inverse p = mass * change_in_velocity
            real impulse = deltaVelocity * totalInvMass;

            // Optional: Clamp the impulse to avoid excessive separation
            real maxImpulse = 1;  // Set an appropriate maximum based on your system
            if (impulse > maxImpulse) {
                impulse = maxImpulse;
            }

            // if(isP2Immovable){
            //     impulse = 0;
            // }

            //std::cout<<" The impulse is : "<<impulse<<std::endl;

            //Impulse needs to be propotional to mass
            Vector3d impulsePerIMass = contactNormal * impulse;

            // Applying the impulses
            // new_velocity = old_velocity + impulse/mass
            particle1->setVelocity(particle1->getVelocity() + impulsePerIMass * particle1->getInverseMass()); 
            //std::cout<<"velocity of particle is " << particle1->getVelocity().x <<" " << particle1->getVelocity().y << " " <<particle1->getVelocity().z<<std::endl;

            if(particle2){
                particle2->setVelocity(particle2->getVelocity() + impulsePerIMass * particle2->getInverseMass());
            }        

        }

        void ParticleContact::resolveInterpenetration(real duration){
            // If we don’t have any penetration, skip this step.
            if (penetration <= 0){
                return;
            }

            real totalInvMass = particle1->getInverseMass();
            if(particle2){
                totalInvMass += particle2->getInverseMass();
            }

            // If all particles have infinite mass, then impulses have no effect.
            if (totalInvMass <= 0){
                return;
            }

            /** Distance particle1 moves is calulated based on
             *  d = (mass_of_particle1 / total_mass_of_1_and_2) * penetration * contact normal
            */

           //first we calulate (1/ total_mass_of_1_and_2) * penetration * contact normal
            Vector3d movePerMass = contactNormal * (penetration / totalInvMass);

            particle1Movement = movePerMass * particle1->getInverseMass();
            if(particle2){
                particle2Movement = movePerMass * particle2->getInverseMass();
            }else{
                particle2Movement.clear();
            }

            // Moving the particles accordingly
            particle1->setPosition(particle1->getPosition() + particle1Movement);

            if(particle2){
                particle2->setPosition(particle2->getPosition() + particle2Movement);
            }

        }


   
        // ParticleContactDetector class definitions
       ParticleContactDetector::ParticleContactDetector() {
            this->ParticleRegistry = nullptr;
            this->maxContacts = 0;
            this->cubeSize = 50.0f; // Default cube size
            this->cubeCenter = Vector3d(0, 0, 0); // Default cube center
        }

        ParticleContactDetector::ParticleContactDetector(std::vector<Particle*>* pr, unsigned maxContacts, real cubeSize, Vector3d cubeCenter) {
            this->ParticleRegistry = pr;
            this->maxContacts = maxContacts;
            this->cubeSize = cubeSize;
            this->cubeCenter = cubeCenter;
        }

        void ParticleContactDetector::setCubeBoundary(real size, Vector3d center) {
            this->cubeSize = size;
            this->cubeCenter = center;
        }

        std::vector<ParticleContact*>& ParticleContactDetector::getContactRegistry() {
            return ParticleContactRegistry;
        }

        unsigned ParticleContactDetector::GenerateContacts() {
            ParticleContactRegistry.clear();
            std::vector<Particle*>::iterator i = (*ParticleRegistry).begin();
            for (; i != (*ParticleRegistry).end() - 1; i++) {
                std::vector<Particle*>::iterator j = (i + 1);
                for (; j != (*ParticleRegistry).end(); j++) {
                    real distance = ((*i)->getPosition() - (*j)->getPosition()).magnitude();
                    real totalRadius = (*i)->getRadius() + (*j)->getRadius();

                    if (distance <= totalRadius) {
                        real penetration = totalRadius - distance;
                        ParticleContactRegistry.push_back(new lancelot::ParticleContact((*i), (*j), 1.0f, penetration));
                    }

                    if (ParticleContactRegistry.size() >= maxContacts) {
                        return 1;
                    }
                }
            }
            return 1;
        }

        void ParticleContactDetector::GenerateWallContacts() {
            real halfSize = cubeSize / 2;

            for (Particle* particle : *ParticleRegistry) {
                Vector3d position = particle->getPosition();

                // Check boundaries and create wall contacts dynamically
                if (position.x > cubeCenter.x + halfSize) {
                    real penetration = position.x - (cubeCenter.x + halfSize);
                    ParticleContactRegistry.push_back(new ParticleContact(particle, nullptr, 1.0f, penetration, Vector3d(-1, 0, 0)));
                }
                if (position.x < cubeCenter.x - halfSize) {
                    real penetration = (cubeCenter.x - halfSize) - position.x;
                    ParticleContactRegistry.push_back(new ParticleContact(particle, nullptr, 1.0f, penetration, Vector3d(1, 0, 0)));
                }

                if (position.y > cubeCenter.y + halfSize) {
                    real penetration = position.y - (cubeCenter.y + halfSize);
                    ParticleContactRegistry.push_back(new ParticleContact(particle, nullptr, 1.0f, penetration, Vector3d(0, -1, 0)));
                }
                if (position.y < cubeCenter.y - halfSize) {
                    real penetration = (cubeCenter.y - halfSize) - position.y;
                    ParticleContactRegistry.push_back(new ParticleContact(particle, nullptr, 1.0f, penetration, Vector3d(0, 1, 0)));
                }

                if (position.z > cubeCenter.z + halfSize) {
                    real penetration = position.z - (cubeCenter.z + halfSize);
                    ParticleContactRegistry.push_back(new ParticleContact(particle, nullptr, 1.0f, penetration, Vector3d(0, 0, -1)));
                }
                if (position.z < cubeCenter.z - halfSize) {
                    real penetration = (cubeCenter.z - halfSize) - position.z;
                    ParticleContactRegistry.push_back(new ParticleContact(particle, nullptr, 1.0f, penetration, Vector3d(0, 0, 1)));
                }
            }
        }


    bool ParticleContactsResolver::ContactComparator::operator()(ParticleContact* a, ParticleContact* b) {
        // Compare based on precomputed separating velocity
        return a->precomputedSeparatingVelocity > b->precomputedSeparatingVelocity;
    }


    // ParticleContactsResolver class definitions
    ParticleContactsResolver::ParticleContactsResolver() : iterations(0), iterationsUsed(0) {}

    ParticleContactsResolver::ParticleContactsResolver(unsigned iterations)
        : iterations(iterations), iterationsUsed(0) {}



    void ParticleContactsResolver::resolveContacts(std::vector<ParticleContact*> contactArray, real duration) {
    // Step 1: Precompute separating velocities for efficiency
    for (ParticleContact* contact : contactArray) {
        contact->precomputedSeparatingVelocity = contact->calculateSeparatingVelocity();
    }

    // Step 2: Create a priority queue to manage contacts by separating velocity
    std::priority_queue<ParticleContact*, std::vector<ParticleContact*>, ContactComparator> contactQueue;

    for (ParticleContact* contact : contactArray) {
        if (!contact->isResolved && contact->precomputedSeparatingVelocity < 0) {
            contactQueue.push(contact);
        }
    }

    // Step 3: Begin resolving contacts
    iterationsUsed = 0;

    while (iterationsUsed < iterations && !contactQueue.empty()) {
        // Get the contact with the largest closing velocity
        ParticleContact* maxContact = contactQueue.top();
        contactQueue.pop();

        if (maxContact->isResolved) continue;

        // Step 4: Resolve this contact
        maxContact->resolve(duration);
        maxContact->isResolved = true;

        // Step 5: Update penetration and related contacts
        Vector3d movement1 = maxContact->particle1Movement;
        Vector3d movement2 = maxContact->particle2Movement;

        for (ParticleContact* contact : contactArray) {
            if (contact->isResolved) continue;

            // Update penetration for contacts involving affected particles
            if (contact->particle1 == maxContact->particle1) {
                contact->penetration -= movement1.dotProduct(contact->contactNormal);
            } else if (contact->particle1 == maxContact->particle2) {
                contact->penetration -= movement2.dotProduct(contact->contactNormal);
            }

            if (contact->particle2) {
                if (contact->particle2 == maxContact->particle1) {
                    contact->penetration += movement1.dotProduct(contact->contactNormal);
                } else if (contact->particle2 == maxContact->particle2) {
                    contact->penetration += movement2.dotProduct(contact->contactNormal);
                }
            }

            // Ensure penetration remains non-negative
            if (contact->penetration < 0) contact->penetration = 0;

            // Recalculate separating velocity if necessary
            contact->precomputedSeparatingVelocity = contact->calculateSeparatingVelocity();

            // Re-add to the priority queue if still unresolved
            if (contact->precomputedSeparatingVelocity < 0) {
                contactQueue.push(contact);
            }
        }

        iterationsUsed++;
    }
}
}
