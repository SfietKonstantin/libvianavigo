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

#ifndef ROUTE_H
#define ROUTE_H

#include "vianavigo_global.h"
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include "place.h"
#include "mode.h"

class RoutePrivate;
class VIANAVIGO_EXPORT Route: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(Place * from READ from CONSTANT)
    Q_PROPERTY(Place * to READ to CONSTANT)
    Q_PROPERTY(QDateTime departureDate READ departureDate CONSTANT)
    Q_PROPERTY(QDateTime arrivalDate READ arrivalDate CONSTANT)
    Q_PROPERTY(int totalTime READ totalTime CONSTANT)
    Q_PROPERTY(int walkingTime READ walkingTime CONSTANT)
    Q_PROPERTY(QList<Mode *> modes READ modes CONSTANT)
    Q_ENUMS(Type)
public:
    enum Type {
        Invalid,
        Faster,
        LessConnections,
        LessWalking,
        NextSchedule,
        MoreWalking,
        OnlyWalking
    };
    explicit Route(QObject *parent = 0);
    virtual ~Route();
    static Route * create(const QString &type, Place *from, Place *to,
                          const QDateTime &departureDate, const QDateTime &arrivalDate,
                          int totalTime, int walkingTime, QList<Mode *> &modes,
                          QObject *parent = 0);
    static Route * copy(Route *other, QObject *parent = 0);
    static Type typeFromString(const QString &type);
    Type type() const;
    Place * from() const;
    Place * to() const;
    QDateTime departureDate() const;
    QDateTime arrivalDate() const;
    int totalTime() const;
    int walkingTime() const;
    QList<Mode *> modes() const;
Q_SIGNALS:
protected:
    explicit Route(RoutePrivate &dd, QObject *parent = 0);
    QScopedPointer<RoutePrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(Route)
};

#endif // ROUTE_H
