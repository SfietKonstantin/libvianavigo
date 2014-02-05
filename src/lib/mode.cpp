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

#include "mode.h"
#include "mode_p.h"

static const char *BUS = "Bus";
static const char *METRO = "Metro";
static const char *RER = "RER";
static const char *TER = "TER";
static const char *TRAIN = "Train";
static const char *TRAM = "Tramway";
static const char *TZEN = "TZen";
static const char *VAL = "Navette";
static const char *WALKING = "Marche";
static const char *WAITING = "Attente";

ModePrivate::ModePrivate(Mode *q):
    type(Mode::Invalid), walkingTime(-1), waitingTime(-1), q_ptr(q)
{
}

Mode::Mode(QObject *parent):
    QObject(parent), d_ptr(new ModePrivate(this))
{
}

Mode::Mode(ModePrivate &dd, QObject *parent):
    QObject(parent), d_ptr(&dd)
{
}

Mode::~Mode()
{
}

Mode * Mode::create(const QString &type, const QString &network, const QString &line,
                    const QString &externalCode, const QString &direction,
                    const QString &departurePlace, const QTime &departureTime, int walkingTime,
                    int waitingTime, QObject *parent)
{
    Mode * returned = new Mode(parent);
    returned->d_func()->type = typeFromString(type);
    returned->d_func()->typeString = type;
    returned->d_func()->network = network;
    returned->d_func()->line = line;
    returned->d_func()->externalCode = externalCode;
    returned->d_func()->direction = direction;
    returned->d_func()->departurePlace = departurePlace;
    returned->d_func()->departureTime = departureTime;
    returned->d_func()->walkingTime = walkingTime;
    returned->d_func()->waitingTime = waitingTime;
    return returned;
}

Mode * Mode::copy(Mode *other, QObject *parent)
{
    return Mode::create(other->d_func()->typeString, other->d_func()->network,
                        other->d_func()->line, other->d_func()->externalCode,
                        other->d_func()->direction, other->d_func()->departurePlace,
                        other->d_func()->departureTime, other->d_func()->walkingTime,
                        other->d_func()->waitingTime, parent);
}

Mode::Type Mode::typeFromString(const QString &type)
{
    Mode::Type returnedType = Mode::Invalid;
    if (type == BUS) {
        returnedType = Mode::Bus;
    } else if (type == METRO) {
        returnedType = Mode::Metro;
    } else if (type == RER) {
        returnedType = Mode::Rer;
    } else if (type == TER) {
        returnedType = Mode::Ter;
    } else if (type == TRAIN) {
        returnedType = Mode::Train;
    } else if (type == TRAM) {
        returnedType = Mode::Tram;
    } else if (type == TZEN) {
        returnedType = Mode::TZen;
    } else if (type == VAL) {
        returnedType = Mode::Val;
    } else if (type == WALKING) {
        returnedType = Mode::Walking;
    } else if (type == WAITING) {
        returnedType = Mode::Waiting;
    }

    return returnedType;
}

Mode::Type Mode::type() const
{
    Q_D(const Mode);
    return d->type;
}

QString Mode::typeString() const
{
    Q_D(const Mode);
    return d->typeString;
}

QString Mode::network() const
{
    Q_D(const Mode);
    return d->network;
}

QString Mode::line() const
{
    Q_D(const Mode);
    return d->line;
}

QString Mode::externalCode() const
{
    Q_D(const Mode);
    return d->externalCode;
}

QString Mode::direction() const
{
    Q_D(const Mode);
    return d->direction;
}

QString Mode::departurePlace() const
{
    Q_D(const Mode);
    return d->departurePlace;
}

QTime Mode::departureTime() const
{
    Q_D(const Mode);
    return d->departureTime;
}

int Mode::walkingTime() const
{
    Q_D(const Mode);
    return d->walkingTime;
}

int Mode::waitingTime() const
{
    Q_D(const Mode);
    return d->waitingTime;
}
