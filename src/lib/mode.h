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

#ifndef MODE_H
#define MODE_H

#include "vianavigo_global.h"
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>

class ModePrivate;
class VIANAVIGO_EXPORT Mode: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(QString network READ network CONSTANT)
    Q_PROPERTY(QString line READ line CONSTANT)
    Q_PROPERTY(QString externalCode READ externalCode CONSTANT)
    Q_PROPERTY(QString direction READ direction CONSTANT)
    Q_PROPERTY(QString departurePlace READ departurePlace CONSTANT)
    Q_PROPERTY(QTime departureTime READ departureTime CONSTANT)
    Q_PROPERTY(int walkingTime READ walkingTime CONSTANT)
    Q_PROPERTY(int waitingTime READ waitingTime CONSTANT)
    Q_ENUMS(Type)
public:
    enum Type {
        Invalid,
        Bus,
        Metro,
        Rer,
        Ter,
        Train,
        Tram,
        TZen,
        Val,
        Walking,
        Waiting
    };
    explicit Mode(QObject *parent = 0);
    virtual ~Mode();
    static Mode * create(const QString &type, const QString &network, const QString &line,
                         const QString &externalCode, const QString &direction,
                         const QString &departurePlace, const QTime &departureTime,
                         int walkingTime, int waitingTime, QObject *parent = 0);
    static Mode * copy(Mode *other, QObject *parent = 0);
    static Type typeFromString(const QString &type);
    Type type() const;
    QString typeString() const;
    QString network() const;
    QString line() const;
    QString externalCode() const;
    QString direction() const;
    QString departurePlace() const;
    QTime departureTime() const;
    int walkingTime() const;
    int waitingTime() const;
Q_SIGNALS:
protected:
    explicit Mode(ModePrivate &dd, QObject *parent = 0);
    QScopedPointer<ModePrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(Mode)
};

#endif // MODE_H
