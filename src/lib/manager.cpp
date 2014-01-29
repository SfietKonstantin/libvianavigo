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

#include "manager.h"
#include <QtCore/QUrlQuery>
#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

static const char *VIANAVIGO_API = "http://www.vianavigo.com/stif_web_carto/rest/";
static const char *VIANAVIGO_PLACE = "searchPoints/";
static const char *VIANAVIGO_ROUTE = "itinerarySearch/";
static const char *DEPARTURE_KEY = "departure";
static const char *DEPARTURE_TYPE_KEY = "departureType";
static const char *ARRIVAL_KEY = "arrival";
static const char *ARRIVAL_TYPE_KEY = "arrivalType";
static const char *DATE_TYPE_KEY = "dateType"; // Seems to be set to 1 for timed search -1 otherwise
static const char *DATE_KEY = "date";
static const char *MODES_KEY = "modes";
static const char *WALK_SPEED_KEY = "walkSpeed";

static const char *DATE_FORMAT = "yyyy-MM-ddThh:mm";
static const char *DATE_TYPE = "1";

class ManagerPrivate
{
public:
    explicit ManagerPrivate(Manager *q);
    static QUrl getSearchRouteUrl(const QString &departure, const QString &departureType,
                                  const QString &arrival, const QString &arrivalType,
                                  const QDateTime &date, const QString &modes,
                                  const QString &walkSpeed);
    QNetworkAccessManager *networkAccessManager;
protected:
    Manager * const q_ptr;
private:
    Q_DECLARE_PUBLIC(Manager)
};

ManagerPrivate::ManagerPrivate(Manager *q)
    : networkAccessManager(0), q_ptr(q)
{
}

QUrl ManagerPrivate::getSearchRouteUrl(const QString &departure, const QString &departureType,
                                       const QString &arrival, const QString &arrivalType,
                                       const QDateTime &date, const QString &modes,
                                       const QString &walkSpeed)
{
    if (departure.isEmpty() || arrival.isEmpty() || date.isNull() || modes.size() != 5
        || walkSpeed.size() != 1) {
        return QUrl();
    }

    QString path = VIANAVIGO_API;
    path.append(VIANAVIGO_ROUTE);
    QUrl url (path);
    QUrlQuery query;
    query.addQueryItem(DEPARTURE_KEY, departure);
    if (!departureType.isEmpty()) {
        query.addQueryItem(DEPARTURE_TYPE_KEY, departureType);
    }
    query.addQueryItem(ARRIVAL_KEY, arrival);
    if (!arrivalType.isEmpty()) {
        query.addQueryItem(ARRIVAL_TYPE_KEY, arrivalType);
    }
    query.addQueryItem(DATE_TYPE_KEY, DATE_TYPE);
    query.addQueryItem(DATE_KEY, date.toString(DATE_FORMAT));
    query.addQueryItem(MODES_KEY, modes);
    query.addQueryItem(WALK_SPEED_KEY, walkSpeed);
    url.setQuery(query);
    return url;
}

Manager::Manager(QObject *parent) :
    QObject(parent), d_ptr(new ManagerPrivate(this))
{
    Q_D(Manager);
    d->networkAccessManager = new QNetworkAccessManager(this);
}

Manager::Manager(ManagerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(Manager);
    d->networkAccessManager = new QNetworkAccessManager(this);
}

Manager::~Manager()
{
}

QNetworkReply * Manager::searchPlace(const QString &text)
{
    Q_D(Manager);
    QString path = VIANAVIGO_API;
    path.append(VIANAVIGO_PLACE);
    path.append(QUrl::toPercentEncoding(text));

    return d->networkAccessManager->get(QNetworkRequest(QUrl(path)));
}

QNetworkReply * Manager::searchRoute(const QString &departure, const QString &arrival,
                                     const QDateTime &date, const QString &modes,
                                     const QString &walkSpeed)
{
    Q_D(Manager);
    QUrl url = ManagerPrivate::getSearchRouteUrl(departure, QString(), arrival, QString(), date,
                                                 modes, walkSpeed);
    if (!url.isEmpty()) {
        return d->networkAccessManager->get(QNetworkRequest(url));
    }
    return 0;
}
