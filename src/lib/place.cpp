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

#include "place.h"
#include "place_p.h"

static const char *ADDRESS = "Address";
static const char *STOP_AREA = "StopArea";
static const char *STOP_POINT = "StopPoint";
static const char *CITY = "City";
static const char *SITE = "Site";

PlacePrivate::PlacePrivate(Place *q):
    q_ptr(q)
{
}

Place::Place(QObject *parent)
    : QObject(parent), d_ptr(new PlacePrivate(this))
{
}

Place::Place(PlacePrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

Place::~Place()
{
}

Place * Place::create(const QString &name, const QString &city, const QString &type,
                      QObject *parent)
{
    Place * returned = new Place(parent);
    returned->d_func()->name = name;
    returned->d_func()->city = city;
    returned->d_func()->type = typeFromString(type);
    returned->d_func()->typeString = type;
    return returned;
}

Place * Place::copy(Place *other, QObject *parent)
{
    return Place::create(other->d_func()->name, other->d_func()->city, other->d_func()->typeString,
                         parent);
}

Place::Type Place::typeFromString(const QString &type)
{
    Place::Type returnedType = Place::Invalid;
    if (type == ADDRESS) {
        returnedType = Place::Address;
    } else if (type == STOP_AREA || type == STOP_POINT) {
        returnedType = Place::Station;
    } else if (type == CITY) {
        returnedType = Place::City;
    } else if (type == SITE) {
        returnedType = Place::POI;
    }

    return returnedType;
}

QString Place::name() const
{
    Q_D(const Place);
    return d->name;
}

QString Place::city() const
{
    Q_D(const Place);
    return d->city;
}

Place::Type Place::type() const
{
    Q_D(const Place);
    return d->type;
}

QString Place::typeString() const
{
    Q_D(const Place);
    return d->typeString;
}
