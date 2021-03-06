/*
  Copyright 2007 Albert Strasheim <fullung@gmail.com>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <boost/python/class.hpp>
#include <boost/python/manage_new_object.hpp>

#include <cms/Connection.h>
#include <cms/ExceptionListener.h>

namespace py = boost::python;

using cms::Connection;
using cms::Startable;
using cms::Stoppable;
using cms::Closeable;
using cms::Session;
using cms::ExceptionListener;

static const char* Connection_docstring =
    "The client's connection to its provider.";
static const char* Connection_clientID_docstring =
    "Return the client id for this session.";
static const char* Connection_exceptionListener_docstring =
    "Gets the registered exception listener for this connection.";
static const char* Connection_createSession0_docstring =
    "Creates an C{AUTO_ACKNOWLEDGE} L{Session}.";
static const char* Connection_createSession1_docstring =
    "Creates a new L{Session} to work for this C{Connection} using the specified acknowledgment mode.";

void export_Connection()
{
    Session* (Connection::*Connection_createSession0)() =
        &Connection::createSession;
    Session* (Connection::*Connection_createSession1)(Session::AcknowledgeMode) =
        &Connection::createSession;

    py::class_<Connection, py::bases<Startable, Stoppable, Closeable>, boost::noncopyable>("Connection", Connection_docstring, py::no_init)
        .add_property("clientID", &Connection::getClientID, Connection_clientID_docstring)
        .add_property("exceptionListener",
                      make_function(&Connection::getExceptionListener, py::return_internal_reference<>()),
                      make_function(&Connection::setExceptionListener, py::with_custodian_and_ward<1, 2>()),
                      Connection_exceptionListener_docstring)
        .def("createSession",
             Connection_createSession0,
             py::return_value_policy<py::manage_new_object, py::with_custodian_and_ward_postcall<0, 1> >(),
             Connection_createSession0_docstring)
        .def("createSession",
             Connection_createSession1,
             py::return_value_policy<py::manage_new_object, py::with_custodian_and_ward_postcall<0, 1> >(),
             py::arg("acknowledgeMode"),
             Connection_createSession1_docstring)
        ;
}
