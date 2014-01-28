/*
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
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

#ifndef ABSTRACTMODEL_P_H
#define ABSTRACTMODEL_P_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QSet>
#include <QtNetwork/QNetworkReply>

class Manager;
class QNetworkReply;
class AbstractModel;
class AbstractModelPrivate: public QObject
{
    Q_OBJECT
public:
    explicit AbstractModelPrivate(AbstractModel *q);
    Manager *manager;
    void setReply(QNetworkReply *reply);
    void clearReply();
    void setLoading(bool loading);
protected:
    virtual void handleFinished(QNetworkReply *reply);
    virtual void handleError(QNetworkReply *reply, QNetworkReply::NetworkError error);
    void clear();
    void addData(const QList<QObject *> &data);
    AbstractModel * const q_ptr;
private Q_SLOTS:
    void slotFinished();
    void slotError(QNetworkReply::NetworkError error);
private:
    bool m_loading;
    QNetworkReply *m_reply;
    QList<QObject *> m_data;
    Q_DECLARE_PUBLIC(AbstractModel)
};


#endif // ABSTRACTMODEL_P_H
