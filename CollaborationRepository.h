#pragma once
#include "Vector.hpp"
#include "Collaboration.h"
#include "MyString.h"

class CollaborationRepository {
private:
    Vector<Collaboration*> collaborations;

public:
    CollaborationRepository() = default;

    ~CollaborationRepository() {
        size_t size = collaborations.getSize();
        for (size_t i = 0; i < size; ++i) {
            delete collaborations[i];
        }
    }

    CollaborationRepository(const CollaborationRepository&) = delete;
    CollaborationRepository& operator=(const CollaborationRepository&) = delete;

    void addCollaboration(Collaboration* collab) {
        collaborations.pushBack(collab);
    }

    Collaboration* findByName(const MyString& name) const {
        size_t size = collaborations.getSize();

        for (size_t i = 0; i < size; ++i) {
            if (collaborations[i]->getId() != -1 && collaborations[i]->getName() == name) {
                return collaborations[i];
            }
        }

        return nullptr;
    }

    const Vector<Collaboration*>& getAllCollaborations() const {
        return collaborations;
    }
};