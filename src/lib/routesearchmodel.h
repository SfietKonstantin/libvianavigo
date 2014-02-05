/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef ROUTESEARCHMODEL_H
#define ROUTESEARCHMODEL_H

#include "vianavigo_global.h"
#include "abstractmodel.h"
#include <QtCore/QDateTime>
#include "place.h"

class RouteSearchModelPrivate;
class VIANAVIGO_EXPORT RouteSearchModel : public AbstractModel
{
    Q_OBJECT
    Q_PROPERTY(Place * departure READ departure WRITE setDeparture NOTIFY departureChanged)
    Q_PROPERTY(Place * arrival READ arrival WRITE setArrival NOTIFY arrivalChanged)
    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(bool useTrain READ useTrain WRITE setUseTrain NOTIFY useTrainChanged)
    Q_PROPERTY(bool useRer READ useRer WRITE setUseRer NOTIFY useRerChanged)
    Q_PROPERTY(bool useMetro READ useMetro WRITE setUseMetro NOTIFY useMetroChanged)
    Q_PROPERTY(bool useTram READ useTram WRITE setUseTram NOTIFY useTramChanged)
    Q_PROPERTY(bool useBus READ useBus WRITE setUseBus NOTIFY useBusChanged)
    Q_PROPERTY(WalkSpeed walkSpeed READ walkSpeed WRITE setWalkSpeed NOTIFY walkSpeedChanged)
    Q_ENUMS(WalkSpeed)
public:
    enum WalkSpeed {
        Slow = 0,
        Normal = 1,
        Fast = 2
    };
    explicit RouteSearchModel(QObject *parent = 0);
    Place * departure() const;
    void setDeparture(Place *departure);
    Place * arrival() const;
    void setArrival(Place *arrival);
    QDateTime date() const;
    void setDate(QDateTime date);
    bool useTrain() const;
    void setUseTrain(bool useTrain);
    bool useRer() const;
    void setUseRer(bool useRer);
    bool useMetro() const;
    void setUseMetro(bool useMetro);
    bool useTram() const;
    void setUseTram(bool useTram);
    bool useBus() const;
    void setUseBus(bool useBus);
    WalkSpeed walkSpeed() const;
    void setWalkSpeed(WalkSpeed walkSpeed);
    QString modesString();
public Q_SLOTS:
    void search();
Q_SIGNALS:
    void departureChanged();
    void arrivalChanged();
    void dateChanged();
    void useTrainChanged();
    void useRerChanged();
    void useMetroChanged();
    void useTramChanged();
    void useBusChanged();
    void walkSpeedChanged();
private:
    Q_DECLARE_PRIVATE(RouteSearchModel)

};

#endif // ROUTESEARCHMODEL_H
