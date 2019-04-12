/****************************************************************************
**
** Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "verto.h"
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QMessageBox>

QT_USE_NAMESPACE

// Look here for info on how to wire up the signal and slot to send messages from this C++ code to the QML that uses it.
// https://github.com/andrewrjones/qml2-to-cpp-and-back-signals

VertoClient::VertoClient(const QUrl &url, QObject *parent) : QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &VertoClient::onConnected);
    connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors), this, &VertoClient::onSslErrors);
    m_webSocket.open(QUrl(url));
}

void VertoClient::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &VertoClient::onTextMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("{\"jsonrpc\":\"2.0\",\"method\":\"login\",\"params\":{\"login\":\"106@\",\"passwd\":\"\",\"loginParams\":\"\",\"userVariables\":\"\",\"sessid\":\"4a18cb88-a135-4bd3-b200-4d74974399ad\"},\"id\":123}"));
}

void VertoClient::onTextMessageReceived(QString message)
{
    if(!m_sent)
        m_webSocket.sendTextMessage(QStringLiteral("{\"jsonrpc\":\"2.0\",\"method\":\"verto.subscribe\",\"params\":{\"eventChannel\":\"\",\"sessid\":null},\"id\":124}"));

    m_sent = true;

    emit messageReceived(message);

    //qApp->quit();
}

void VertoClient::onSslErrors(const QList<QSslError> &errors)
{
    printf("Ssl error\n");

    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    m_webSocket.ignoreSslErrors();
}
