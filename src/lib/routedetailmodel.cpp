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

#include "routedetailmodel.h"
#include "abstractmodel_p.h"
#include "routeconstants_p.h"
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

static const char *RESULT_KEY = "result";
static const char *ZONES_KEY = "zones";
static const char *STEPS_KEY = "iti";
static const char *STEP_DIRECTION_KEY = "dir";
static const char *STEP_DEPARTURE_PLACE_KEY = "nom";
static const char *STEP_DEPARTURE_TIME_KEY = "depart";
static const char *STEP_WALKING_TIME_KEY = "walkDuration";
static const char *STEP_WAITING_TIME_KEY = "waitDuration";

class RouteDetailModelPrivate: public AbstractModelPrivate
{
public:
    explicit RouteDetailModelPrivate(RouteDetailModel *q);
    RouteSearchModel *routeSearchModel;
    Route::Type routeType;
    Route *route;
protected:
    void handleFinished(QNetworkReply *reply);
private:
    Q_DECLARE_PUBLIC(RouteDetailModel)
};

RouteDetailModelPrivate::RouteDetailModelPrivate(RouteDetailModel *q)
    : AbstractModelPrivate(q), routeSearchModel(0), routeType(Route::Invalid), route(0)
{
}

void RouteDetailModelPrivate::handleFinished(QNetworkReply *reply)
{
    Q_Q(RouteDetailModel);
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse result:" << error.errorString();
        clearReply();
        setStatus(AbstractModel::Error);
        return;
    }

    QJsonObject result = document.object().value(RESULT_KEY).toObject();
    QString departureTime = result.value(DEPARTURE_TIME_KEY).toString();
    QString departureName = result.value(DEPARTURE_NAME_KEY).toString();
    QString departureType = result.value(DEPARTURE_TYPE_KEY).toString();

    QString arrivalTime = result.value(ARRIVAL_TIME_KEY).toString();
    QString arrivalName = result.value(ARRIVAL_NAME_KEY).toString();
    QString arrivalType = result.value(ARRIVAL_TYPE_KEY).toString();

    int totalTime = result.value(TOTAL_TIME_KEY).toDouble();
    QString zones = result.value(ZONES_KEY).toString();

    route = Route::create(QString(), Place::create(departureName, QString(), departureType),
                          Place::create(arrivalName, QString(), arrivalType),
                          Manager::getDate(departureTime), Manager::getDate(arrivalTime),
                          totalTime, -1, zones, QList<Mode *>(), this);

    QJsonArray stepsArray = result.value(STEPS_KEY).toArray();
    QList<QObject *> steps;
    foreach (const QJsonValue &stepEntry, stepsArray) {
        QJsonObject stepObject = stepEntry.toObject();
        QString stepType = stepObject.value(MODE_TYPE_KEY).toString();
        QString network = stepObject.value(MODE_NETWORK_KEY).toString();
        QString line = stepObject.value(MODE_LINE_KEY).toString();
        QString externalCode = stepObject.value(MODE_EXTERNAL_CODE_KEY).toString();
        QString direction = stepObject.value(STEP_DIRECTION_KEY).toString();
        QString departurePlace = stepObject.value(STEP_DEPARTURE_PLACE_KEY).toString();
        QTime departureTime = QTime::fromString(stepObject.value(STEP_DEPARTURE_TIME_KEY).toString(),
                                                "hh:MM");
        int walkingTime = stepObject.value(STEP_WALKING_TIME_KEY).toDouble(-1);
        int waitingTime = stepObject.value(STEP_WAITING_TIME_KEY).toDouble(-1);
        Mode *step = Mode::create(stepType, network, line, externalCode, direction, departurePlace,
                                  departureTime, walkingTime, waitingTime);
        steps.append(step);
    }
    emit q->routeChanged();
    addData(steps);
    clearReply();
    setStatus(AbstractModel::Idle);
}

RouteDetailModel::RouteDetailModel(QObject *parent) :
    AbstractModel(*(new RouteDetailModelPrivate(this)), parent)
{
}

RouteSearchModel * RouteDetailModel::routeSearchModel() const
{
    Q_D(const RouteDetailModel);
    return d->routeSearchModel;
}

void RouteDetailModel::setRouteSearchModel(RouteSearchModel *routeSearchModel)
{
    Q_D(RouteDetailModel);
    if (d->routeSearchModel != routeSearchModel) {
        d->routeSearchModel = routeSearchModel;
        emit routeSearchModelChanged();
    }
}

Route::Type RouteDetailModel::routeType() const
{
    Q_D(const RouteDetailModel);
    return d->routeType;
}

void RouteDetailModel::setRouteType(Route::Type routeType)
{
    Q_D(RouteDetailModel);
    if (d->routeType != routeType) {
        d->routeType = routeType;
        emit routeTypeChanged();
    }
}

Route * RouteDetailModel::route() const
{
    Q_D(const RouteDetailModel);
    return d->route;
}

void RouteDetailModel::search()
{
    Q_D(RouteDetailModel);
    if (!d->manager) {
        qWarning() << "Manager is not set.";
        return;
    }

    if (!d->routeSearchModel) {
        qWarning() << "RouteSearchModel is not set";
        return;
    }

    if (d->routeType == Route::Invalid) {
        qWarning() << "The type of route to fetch is not set";
        return;
    }

    QNetworkReply *reply = d->manager->searchRoute(d->routeSearchModel->departure()->name(),
                                                   d->routeSearchModel->departure()->typeString(),
                                                   d->routeSearchModel->arrival()->name(),
                                                   d->routeSearchModel->arrival()->typeString(),
                                                   d->routeSearchModel->date(),
                                                   d->routeSearchModel->modesString(),
                                                   QString::number(d->routeSearchModel->walkSpeed()),
                                                   d->routeType);
    if (reply) {
        if (d->route) {
            d->route->deleteLater();
            d->route = 0;
        }
        d->clear();
        d->setReply(reply);
    }
}
