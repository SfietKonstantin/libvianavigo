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

#include "searchresult.h"
#include "searchresult_p.h"

SearchResultPrivate::SearchResultPrivate(SearchResult *q):
    q_ptr(q)
{
}

SearchResult::SearchResult(QObject *parent)
    : QObject(parent), d_ptr(new SearchResultPrivate(this))
{
}

SearchResult::SearchResult(SearchResultPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

SearchResult::~SearchResult()
{
}

SearchResult * SearchResult::create(const QString &name, const QString &city, const QString &type, const QString &typeLabel, const QString &search, const QString &externalCode, const QString &cityCode, QObject *parent)
{
    SearchResult * returned = new SearchResult(parent);
    returned->d_func()->name = name;
    returned->d_func()->city = city;
    returned->d_func()->type = type;
    returned->d_func()->typeLabel = typeLabel;
    returned->d_func()->search = search;
    returned->d_func()->externalCode = externalCode;
    returned->d_func()->cityCode = cityCode;
    return returned;
}

QString SearchResult::name() const
{
    Q_D(const SearchResult);
    return d->name;
}

QString SearchResult::city() const
{
    Q_D(const SearchResult);
    return d->city;
}

QString SearchResult::type() const
{
    Q_D(const SearchResult);
    return d->type;
}

QString SearchResult::typeLabel() const
{
    Q_D(const SearchResult);
    return d->typeLabel;
}

QString SearchResult::search() const
{
    Q_D(const SearchResult);
    return d->search;
}

QString SearchResult::externalCode() const
{
    Q_D(const SearchResult);
    return d->externalCode;
}

QString SearchResult::cityCode() const
{
    Q_D(const SearchResult);
    return d->cityCode;
}

