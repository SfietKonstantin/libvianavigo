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

#include "route.h"
#include "route_p.h"

static const char *FASTER = "1";
static const char *LESS_CONNECTIONS = "2";
static const char *LESS_WALKING = "3";
static const char *NEXT_SCHEDULE = "4";
static const char *MORE_WALKING = "5";
static const char *ONLY_WALKING = "6";

RoutePrivate::RoutePrivate(Route *q):
    q_ptr(q)
{
}

Route::Route(QObject *parent):
    QObject(parent), d_ptr(new RoutePrivate(this))
{
}

Route::Route(RoutePrivate &dd, QObject *parent):
    QObject(parent), d_ptr(&dd)
{
}

Route::~Route()
{
}

Route * Route::create(const QString &type, Place *from, Place *to, const QDateTime &departureDate,
                      const QDateTime &arrivalDate, int totalTime, int walkingTime,
                      const QString &zones, const QList<Mode *> &modes, QObject *parent)
{
    Route * returned = new Route(parent);
    returned->d_func()->type = typeFromString(type);
    returned->d_func()->typeString = type;
    from->setParent(returned);
    returned->d_func()->from = from;
    to->setParent(returned);
    returned->d_func()->to = to;
    returned->d_func()->departureDate = departureDate;
    returned->d_func()->arrivalDate = arrivalDate;
    returned->d_func()->totalTime = totalTime;
    returned->d_func()->walkingTime = walkingTime;
    returned->d_func()->zones = zones;
    foreach (Mode *mode, modes) {
        mode->setParent(mode);
    }
    returned->d_func()->modes = modes;
    return returned;
}

Route * Route::copy(Route *other, QObject *parent)
{
    // Warning: pointers will be shared
    return Route::create(other->d_func()->typeString, other->d_func()->from, other->d_func()->to,
                         other->d_func()->departureDate, other->d_func()->arrivalDate,
                         other->d_func()->totalTime, other->d_func()->walkingTime,
                         other->d_func()->zones, other->d_func()->modes, parent);
}

Route::Type Route::typeFromString(const QString &type)
{
    Route::Type returnedType = Route::Invalid;
    if (type == FASTER) {
        returnedType = Faster;
    } else if (type == LESS_CONNECTIONS) {
        returnedType = LessConnections;
    } else if (type == LESS_WALKING) {
        returnedType = LessWalking;
    } else if (type == NEXT_SCHEDULE) {
        returnedType = NextSchedule;
    } else if (type == MORE_WALKING) {
        returnedType = MoreWalking;
    } else if (type == ONLY_WALKING) {
        returnedType = OnlyWalking;
    }

    return returnedType;
}

Route::Type Route::type() const
{
    Q_D(const Route);
    return d->type;
}

Place * Route::from() const
{
    Q_D(const Route);
    return d->from;
}

Place * Route::to() const
{
    Q_D(const Route);
    return d->to;
}

QDateTime Route::departureDate() const
{
    Q_D(const Route);
    return d->departureDate;
}

QDateTime Route::arrivalDate() const
{
    Q_D(const Route);
    return d->arrivalDate;
}

int Route::totalTime() const
{
    Q_D(const Route);
    return d->totalTime;
}

int Route::walkingTime() const
{
    Q_D(const Route);
    return d->walkingTime;
}

QString Route::zones() const
{
    Q_D(const Route);
    return d->zones;
}

QList<Mode *> Route::modes() const
{
    Q_D(const Route);
    return d->modes;
}

QList<QObject *> Route::modeObjects() const
{
    Q_D(const Route);
    QList<QObject *> modes;
    foreach (Mode *mode, d->modes) {
        modes.append(mode);
    }
    return modes;
}
