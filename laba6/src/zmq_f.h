#ifndef LAB6_ZMQ_F_H
#define LAB6_ZMQ_F_H

#include <iostream>
#include <string>
#include <zmq.hpp>

const int MAIN_PORT = 8000;

void send_message(zmq::socket_t& socket, const std::string& msg)
{
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message);
}

std::string receive_message(zmq::socket_t& socket)
{
    zmq::message_t message;
    int chars_read;
    try {
        chars_read = (int)socket.recv(&message);
    }
    catch (...) {
        chars_read = 0;
    }
    if (chars_read == 0) {
        return "Error"; 
    }
    std::string received_msg(static_cast<char*>(message.data()), message.size());
    return received_msg;
}

void connect(zmq::socket_t& socket, int id)
{
    std::string adress = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.connect(adress);
}

void disconnect(zmq::socket_t& socket, int id)
{
    std::string adress = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.disconnect(adress);
}

void bind(zmq::socket_t& socket, int id)
{
    std::string adress = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.bind(adress);
}

void unbind(zmq::socket_t& socket, int id)
{
    std::string adress = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.unbind(adress);
}

#endif