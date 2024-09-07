#include "Producto.h"

QDataStream& operator>>(QDataStream& in, Producto& producto) {
    in >> producto.codigo >> producto.nombre >> producto.precio >> producto.cantidadStock;
    return in;
}

QDataStream& operator<<(QDataStream& out, const Producto& producto) {
    out << producto.codigo << producto.nombre << producto.precio << producto.cantidadStock;
    return out;
}

QDebug operator<<(QDebug debug, const Producto &producto) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "Producto(id: " << producto.codigo
                    << ", nombre: " << producto.nombre
                    << ", precio: " << producto.precio
                    << ", cantidad: " << producto.cantidadStock << ")";
    return debug;
}
