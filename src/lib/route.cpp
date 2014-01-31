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

#include "route.h"
#include "route_p.h"

RoutePrivate::RoutePrivate(Route *q):
    from(0), to(0), q_ptr(q)
{
}

Route::Route(QObject *parent)
    : QObject(parent), d_ptr(new RoutePrivate(this))
{
}

Route::Route(RoutePrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

Route::~Route()
{
}

Route * Route::create(Place *from, Place *to, QObject *parent)
{
    Route * returned = new Route(parent);
    returned->d_func()->from = from;
    returned->d_func()->to = to;
    return returned;
}

Route * Route::copy(Route *route, QObject *parent)
{
    Route * returned = new Route(parent);
    returned->d_func()->from = route->d_func()->from;
    returned->d_func()->to = route->d_func()->to;
    return returned;
}

Place * Route::from() const
{
    Q_D(const Route);
    return d->from;
}

Place * Route::to() const
{
    Q_D(const Route);
    return d->to;
}

