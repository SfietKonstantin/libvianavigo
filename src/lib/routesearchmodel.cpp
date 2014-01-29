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

#include "routesearchmodel.h"
#include "abstractmodel_p.h"
#include "route.h"
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

static const char *USE = "1";
static const char *DONT_USE = "0";
static const char *RESULTS_KEY = "results";

class RouteSearchModelPrivate: public AbstractModelPrivate
{
public:
    explicit RouteSearchModelPrivate(RouteSearchModel *q);
    QString modesToString() const;
    Place * departure;
    Place * arrival;
    QDateTime date;
    bool useTrain;
    bool useRer;
    bool useMetro;
    bool useTram;
    bool useBus;
    RouteSearchModel::WalkSpeed walkSpeed;
protected:
    void handleFinished(QNetworkReply *reply);
};

RouteSearchModelPrivate::RouteSearchModelPrivate(RouteSearchModel *q)
    : AbstractModelPrivate(q), departure(0), arrival(0), useTrain(true), useRer(true)
    , useMetro(true), useTram(true), useBus(true), walkSpeed(RouteSearchModel::Normal)
{
}

QString RouteSearchModelPrivate::modesToString() const
{
    // The order of entries are
    // 1. train
    // 2. RER
    // 3. metro
    // 4. tram
    // 5. bus
    QString modes;
    modes.append(useTrain ? USE : DONT_USE);
    modes.append(useRer ? USE : DONT_USE);
    modes.append(useMetro ? USE : DONT_USE);
    modes.append(useTram ? USE : DONT_USE);
    modes.append(useBus ? USE : DONT_USE);
    return modes;
}

void RouteSearchModelPrivate::handleFinished(QNetworkReply *reply)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse result:" << error.errorString();
        clearReply();
        setStatus(AbstractModel::Error);
        return;
    }

    QList<QObject *> places;
    QJsonArray list = document.object().value(RESULTS_KEY).toArray();
    foreach (const QJsonValue &entry, list) {
        qDebug() << entry;
    }
    addData(places);
    clearReply();
    setStatus(AbstractModel::Idle);
}

RouteSearchModel::RouteSearchModel(QObject *parent) :
    AbstractModel(*(new RouteSearchModelPrivate(this)), parent)
{
}

Place * RouteSearchModel::departure() const
{
    Q_D(const RouteSearchModel);
    return d->departure;
}

void RouteSearchModel::setDeparture(Place *departure)
{
    Q_D(RouteSearchModel);
    if (d->departure != departure) {
        d->departure = departure;
        emit departureChanged();
    }
}

Place * RouteSearchModel::arrival() const
{
    Q_D(const RouteSearchModel);
    return d->arrival;
}

void RouteSearchModel::setArrival(Place *arrival)
{
    Q_D(RouteSearchModel);
    if (d->arrival != arrival) {
        d->arrival = arrival;
        emit arrivalChanged();
    }
}

QDateTime RouteSearchModel::date() const
{
    Q_D(const RouteSearchModel);
    return d->date;
}

void RouteSearchModel::setDate(QDateTime date)
{
    Q_D(RouteSearchModel);
    if (d->date != date) {
        d->date = date;
        emit dateChanged();
    }
}

bool RouteSearchModel::useTrain() const
{
    Q_D(const RouteSearchModel);
    return d->useTrain;
}

void RouteSearchModel::setUseTrain(bool useTrain)
{
    Q_D(RouteSearchModel);
    if (d->useTrain != useTrain) {
        d->useTrain = useTrain;
        emit useTrainChanged();
    }
}

bool RouteSearchModel::useRer() const
{
    Q_D(const RouteSearchModel);
    return d->useRer;
}

void RouteSearchModel::setUseRer(bool useRer)
{
    Q_D(RouteSearchModel);
    if (d->useRer != useRer) {
        d->useRer = useRer;
        emit useRerChanged();
    }
}

bool RouteSearchModel::useMetro() const
{
    Q_D(const RouteSearchModel);
    return d->useMetro;
}

void RouteSearchModel::setUseMetro(bool useMetro)
{
    Q_D(RouteSearchModel);
    if (d->useMetro != useMetro) {
        d->useMetro = useMetro;
        emit useMetroChanged();
    }
}

bool RouteSearchModel::useTram() const
{
    Q_D(const RouteSearchModel);
    return d->useTram;
}

void RouteSearchModel::setUseTram(bool useTram)
{
    Q_D(RouteSearchModel);
    if (d->useTram != useTram) {
        d->useTram = useTram;
        emit useTramChanged();
    }
}

bool RouteSearchModel::useBus() const
{
    Q_D(const RouteSearchModel);
    return d->useBus;
}

void RouteSearchModel::setUseBus(bool useBus)
{
    Q_D(RouteSearchModel);
    if (d->useBus != useBus) {
        d->useBus = useBus;
        emit useBusChanged();
    }
}

RouteSearchModel::WalkSpeed RouteSearchModel::walkSpeed() const
{
    Q_D(const RouteSearchModel);
    return d->walkSpeed;
}

void RouteSearchModel::setWalkSpeed(WalkSpeed walkSpeed)
{
    Q_D(RouteSearchModel);
    if (d->walkSpeed != walkSpeed) {
        d->walkSpeed = walkSpeed;
        emit walkSpeedChanged();
    }
}

void RouteSearchModel::search()
{
    Q_D(RouteSearchModel);
    if (!d->manager) {
        qWarning() << "Manager is not set.";
        return;
    }

    if (!d->departure) {
        qWarning() << "Departure place is not set";
        return;
    }

    if (!d->arrival) {
        qWarning() << "Departure place is not set";
        return;
    }

    if (d->date.isNull()) {
        qWarning() << "Travel date is not set";
        return;
    }

    QNetworkReply *reply = d->manager->searchRoute(d->departure->name(), d->arrival->name(),
                                                   d->date, d->modesToString(),
                                                   QString::number(d->walkSpeed));
    if (reply) {
        d->clear();
        d->setReply(reply);
    }
}
