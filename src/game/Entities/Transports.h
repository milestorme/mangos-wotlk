/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TRANSPORTS_H
#define TRANSPORTS_H

#include "Entities/GameObject.h"
#include "Maps/TransportMgr.h"

#include <map>
#include <set>

typedef std::set<WorldObject*> PassengerSet;

class GenericTransport : public GameObject
{
    public:
        GenericTransport() : m_passengerTeleportIterator(m_passengers.end()), m_pathProgress(0), m_movementStarted(0), m_stopped(false){}
        bool AddPassenger(WorldObject* passenger);
        bool RemovePassenger(WorldObject* passenger);
        bool AddPetToTransport(Unit* passenger, Pet* pet);

        void UpdatePosition(float x, float y, float z, float o);
        void UpdatePassengerPosition(WorldObject* object);

        typedef std::set<Player*> PlayerSet;
        PassengerSet& GetPassengers() { return m_passengers; }

        /// This method transforms supplied transport offsets into global coordinates
        void CalculatePassengerPosition(float& x, float& y, float& z, float* o = nullptr) const
        {
            CalculatePassengerPosition(x, y, z, o, GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());
        }

        /// This method transforms supplied global coordinates into local offsets
        void CalculatePassengerOffset(float& x, float& y, float& z, float* o = nullptr) const
        {
            CalculatePassengerOffset(x, y, z, o, GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());
        }

        void CalculatePassengerOrientation(float& o) const;

        static void CalculatePassengerPosition(float& x, float& y, float& z, float* o, float transX, float transY, float transZ, float transO);
        static void CalculatePassengerOffset(float& x, float& y, float& z, float* o, float transX, float transY, float transZ, float transO);

        uint32 GetPathProgress() const { return m_pathProgress; }

        void SetGoState(GOState state) override;
    protected:
        void UpdatePassengerPositions(PassengerSet& passengers);

        PassengerSet m_passengers;
        PassengerSet::iterator m_passengerTeleportIterator;

        uint32 m_pathProgress; // for MO transport its full time since start for normal time in cycle
        uint32 m_movementStarted;
        bool m_stopped;
};

class ElevatorTransport : public GenericTransport
{
    public:
        bool Create(uint32 guidlow, uint32 name_id, Map* map, uint32 phaseMask, float x, float y, float z, float ang,
            const QuaternionData& rotation = QuaternionData(), uint8 animprogress = GO_ANIMPROGRESS_DEFAULT, GOState go_state = GO_STATE_READY) override;
        void Update(const uint32 diff) override;

        void SetGoState(GOState state) override;
    private:
        TransportAnimation const* m_animationInfo;
        uint32 m_currentSeg;
        bool m_eventTriggered;
};

class Transport : public GenericTransport
{
    public:
        explicit Transport(TransportTemplate const& transportTemplate);
        
        static bool IsSpawnedByDefault(uint32 entry, Team team);
        static void LoadTransport(TransportTemplate const& transportTemplate, Map* map);
        bool Create(uint32 guidlow, uint32 mapid, float x, float y, float z, float ang, uint8 animprogress, uint16 dynamicHighValue);
        void Update(const uint32 diff) override;

        uint32 GetPeriod() const { return m_period; }
        void SetPeriod(uint32 period) { m_period = period; }

        KeyFrameVec const& GetKeyFrames() const { return m_transportTemplate.keyFrames; }

        void SpawnPassengers();
    private:
        void TeleportTransport(uint32 newMapid, float x, float y, float z, float o);
        void UpdateForMap(Map const* targetMap, bool newMap);
        void DoEventIfAny(TaxiPathNodeEntry const& node, bool departure);
        void MoveToNextWayPoint();                          // move m_next/m_cur to next points
        float CalculateSegmentPos(float perc);

        bool IsMoving() const { return m_isMoving; }
        void SetMoving(bool val) { m_isMoving = val; }

        ShortTimeTracker m_positionChangeTimer;
        bool m_isMoving;

        KeyFrameVec::const_iterator m_currentFrame;
        KeyFrameVec::const_iterator m_nextFrame;
        uint32 m_pathTime;

        uint32 m_period;
        int32 m_lastStopIndex;

        TransportTemplate const& m_transportTemplate;
};
#endif
