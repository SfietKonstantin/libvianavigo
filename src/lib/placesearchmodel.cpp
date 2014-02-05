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

#include "placesearchmodel.h"
#include "abstractmodel_p.h"
#include "place.h"
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

static const int MIN_TEXT_SIZE = 3;

static const char *LIST_KEY = "list";
static const char *NAME_KEY = "name";
static const char *CITY_KEY = "city";
static const char *TYPE_KEY = "type";

class PlaceSearchModelPrivate: public AbstractModelPrivate
{
public:
    explicit PlaceSearchModelPrivate(PlaceSearchModel *q);
    QString text;
protected:
    void handleFinished(QNetworkReply *reply);
};

PlaceSearchModelPrivate::PlaceSearchModelPrivate(PlaceSearchModel *q)
    : AbstractModelPrivate(q)
{
}

void PlaceSearchModelPrivate::handleFinished(QNetworkReply *reply)
{
    QJsonParseError error;
    QByteArray data = reply->readAll();
    if (data.isEmpty()) {
        clearReply();
        setStatus(AbstractModel::Idle);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse result:" << error.errorString();
        clearReply();
        setStatus(AbstractModel::Error);
        return;
    }

    QList<QObject *> places;
    QJsonArray list = document.object().value(LIST_KEY).toArray();
    foreach (const QJsonValue &entry, list) {
        QJsonObject place = entry.toObject();
        QString name = place.value(NAME_KEY).toString();
        QString city = place.value(CITY_KEY).toString();
        QString typeString = place.value(TYPE_KEY).toString();
        Place::Type type = Place::typeFromString(typeString);

        if (type != Place::Invalid) {
            places.append(Place::create(name, city, typeString, this));
        }
    }
    addData(places);
    clearReply();
    setStatus(AbstractModel::Idle);
}

PlaceSearchModel::PlaceSearchModel(QObject *parent) :
    AbstractModel(*(new PlaceSearchModelPrivate(this)), parent)
{
}

QString PlaceSearchModel::text() const
{
    Q_D(const PlaceSearchModel);
    return d->text;
}

void PlaceSearchModel::setText(const QString &text)
{
    Q_D(PlaceSearchModel);
    if (d->text != text) {
        d->text = text;
        emit textChanged();

        if (!d->manager) {
            qWarning() << "Manager is not set.";
            return;
        }

        d->clear();
        if (d->text.trimmed().size() < MIN_TEXT_SIZE) {
            return;
        }
        d->setReply(d->manager->searchPlace(d->text.trimmed()));
    }
}

