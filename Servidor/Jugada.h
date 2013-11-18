#ifndef JUGADA_H
#define JUGADA_H


class Jugada {
    public:
        Jugada();
        virtual ~Jugada();
        bool sumarPuntos(int cant_borrados, bool con_estrella);
        int Jugada::verPuntos();
        Dimension posicion1();
        Dimension posicion2();
    protected:
    private:
        int puntos;
        Dimension* pos1;
        Dimension* pos2;
};

#endif // JUGADA_H
