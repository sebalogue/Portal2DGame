//
// Created by santi on 09/06/19.
//

#ifndef COLABLOQUEANTE_H
#define COLABLOQUEANTE_H

#include <queue>
#include <condition_variable>

template<class T>
class ColaBloqueante {
private:
    std::queue<T> cola;
    std::mutex m;
    std::condition_variable cond_var;
    bool done;
    bool notified;
public:
    ColaBloqueante() : done(false), notified(false) {

    }
    // Encola y notifica a un thread para que este
    // desencole.
    void push(T elemento) {
        std::unique_lock<std::mutex> lock(m);
        cola.push(std::move(elemento));
        notified = true;
        cond_var.notify_one();
    }

    // Espera a poder desencolar . Si puede desencolar, devuelve
    // T. En caso de no haber mas nada para desencolar
    // se devuelve nullptr.
    bool pop(T &elemento) {
        std::unique_lock<std::mutex> lock(m);
        while (!notified && !done) {
            cond_var.wait(lock);
        }
        if (!cola.empty()) {
            elemento = std::move(cola.front());
            cola.pop();
            this->notified = false;
            return true;
        }
        return false;
        //return done;
    }
    // Se notifica a todos los threads que ya
    // no hay mas nada para desencolar.
    void finalizado() {
        std::unique_lock<std::mutex> lock(this->m);
        done = true;
        cond_var.notify_all();
    }

    ~ColaBloqueante() = default;
};


#endif //COLABLOQUEANTE_H