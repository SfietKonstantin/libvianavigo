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

#include "searchmodel.h"
#include "abstractmodel_p.h"
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonParseError>

static const char *LIST_KEY = "list";

class SearchModelPrivate: public AbstractModelPrivate
{
public:
    explicit SearchModelPrivate(SearchModel *q);
    QString text;
protected:
    void handleFinished(QNetworkReply *reply);
};

SearchModelPrivate::SearchModelPrivate(SearchModel *q)
    : AbstractModelPrivate(q)
{
}

void SearchModelPrivate::handleFinished(QNetworkReply *reply)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse result:" << error.errorString();
        clearReply();
        setLoading(false);
        return;
    }

    QJsonArray list = document.object().value(LIST_KEY).toArray();
    foreach (const QJsonValue &entry, list) {
        qDebug() << entry.toObject().value("name").toString();
    }

    clearReply();
    setLoading(false);
}

SearchModel::SearchModel(QObject *parent) :
    AbstractModel(*(new SearchModelPrivate(this)), parent)
{
}

QString SearchModel::text() const
{
    Q_D(const SearchModel);
    return d->text;
}

void SearchModel::setText(const QString &text)
{
    Q_D(SearchModel);
    if (d->text != text) {
        d->text = text;
        emit textChanged();

        if (!d->manager) {
            qWarning() << "Manager is not set.";
            return;
        }
        d->setReply(d->manager->search(d->text));
    }
}

