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
#include "routeconstants_p.h"
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

static const char *USE = "1";
static const char *DONT_USE = "0";
static const char *RESULTS_KEY = "results";
static const char *TYPE_KEY = "type";
static const char *WALKING_TIME_KEY = "MAP";
static const char *MODES_KEY = "etapes";
static const char *MODE_EXTERNAL_CODE_KEY = "externalCode";

class RouteSearchModelPrivate: public AbstractModelPrivate
{
public:
    explicit RouteSearchModelPrivate(RouteSearchModel *q);
    QString modesString() const;
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

QString RouteSearchModelPrivate::modesString() const
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

    QList<QObject *> routes;
    QJsonArray list = document.object().value(RESULTS_KEY).toArray();
    foreach (const QJsonValue &entry, list) {
        QJsonObject route = entry.toObject();
        QString type = route.value(TYPE_KEY).toString();
        QString departureTime = route.value(DEPARTURE_TIME_KEY).toString();
        QString departureName = route.value(DEPARTURE_NAME_KEY).toString();
        QString departureType = route.value(DEPARTURE_TYPE_KEY).toString();

        QString arrivalTime = route.value(ARRIVAL_TIME_KEY).toString();
        QString arrivalName = route.value(ARRIVAL_NAME_KEY).toString();
        QString arrivalType = route.value(ARRIVAL_TYPE_KEY).toString();

        int totalTime = route.value(TOTAL_TIME_KEY).toDouble();
        // MAP is a string
        int walkingTime = route.value(WALKING_TIME_KEY).toString().toInt();

        QJsonArray modesArray = route.value(MODES_KEY).toArray();
        QList<Mode *> modes;
        foreach (const QJsonValue &modeEntry, modesArray) {
            QJsonObject modeObject = modeEntry.toObject();
            QString modeType = modeObject.value(MODE_TYPE_KEY).toString();
            QString network = modeObject.value(MODE_NETWORK_KEY).toString();
            QString line = modeObject.value(MODE_LINE_KEY).toString();
            QString externalCode = modeObject.value(MODE_EXTERNAL_CODE_KEY).toString();
            Mode *mode = Mode::create(modeType, network, line, externalCode, QString(), QString(),
                                      QTime(), -1, -1);
            modes.append(mode);
        }
        routes.append(Route::create(type, Place::create(departureName, QString(), departureType),
                                    Place::create(arrivalName, QString(), arrivalType),
                                    Manager::getDate(departureTime), Manager::getDate(arrivalTime),
                                    totalTime, walkingTime, QString(), modes, this));
    }
    addData(routes);
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

QString RouteSearchModel::modesString()
{
    Q_D(RouteSearchModel);
    return d->modesString();
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
        qWarning() << "Arrival place is not set";
        return;
    }

    if (d->date.isNull()) {
        qWarning() << "Travel date is not set";
        return;
    }

    QNetworkReply *reply = d->manager->searchRoute(d->departure->name(), d->departure->typeString(),
                                                   d->arrival->name(), d->arrival->typeString(),
                                                   d->date, d->modesString(),
                                                   QString::number(d->walkSpeed));
    if (reply) {
        d->clear();
        d->setReply(reply);
    }
}
