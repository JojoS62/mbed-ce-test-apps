/* 
 * Copyright (c) 2019 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "WebsocketHandlers.h"
#include "ClientConnection.h"

void WSHandler::onMessage(const char* text)
{
    // printf("ws onMessage: '%s'\n", text);
}

void WSHandler::onMessage(const char* data, size_t size)
{
}

void WSHandler::onOpen(ClientConnection *clientConnection)
{
    WebSocketHandler::onOpen(clientConnection);
    debug("%s: websocket opened\n", _clientConnection->getThreadname());
    _valX = 0;
}
 
void WSHandler::onClose()
{
    debug("%s: websocket closed\n", _clientConnection->getThreadname());
}


// WSHandler Factory
WebSocketHandler* WSHandler::createHandler()
{
    WebSocketHandler* handler = new WSHandler();
    return handler;
}
