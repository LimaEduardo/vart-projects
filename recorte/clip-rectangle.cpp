#include "clip-rectangle.h"
#include "point.h"
#include <iostream>
#include <cmath>

using namespace std;

ClipRectangle::ClipRectangle (double xmin, double xmax, double ymin, double ymax)
    : mXmin(xmin), mXmax(xmax), mYmin(ymin), mYmax(ymax), mInitialized(true)
{}

void ClipRectangle::Set(double xmin, double xmax, double ymin, double ymax)
{
    mXmin = xmin;
    mXmax = xmax;
    mYmin = ymin;
    mYmax = ymax;
    mInitialized = true;
}

// Retorna um ponteiro para uma linha recem alocada inteiramente dentro da area de recorte
// ou NULL se l for completamente descartada.
// A linha alocada aqui será desalocada na função CleanUp().
Line* ClipRectangle::Clip(const Line& l)
{
    // incicializar a posição de inicio com 0
    // inciialiazar a posição de fim com 1
    double inicio = 0;
    double fim = 1;
    Point p0 = l.mP0;
    Point p1 = l.mP1;
    // 0 - Esquerda
    // 1 - Direita
    // 2 - Superior
    // 3 - Inferior
    // Para cada lado da área de recorte
    for (int i = 0; i < 4; ++i){
        double numerador, denominador;
        // Calcula o numerador e o denominador
        switch(i){
            //Esquerda
            case 0:
                numerador = this->mXmin - p0.mX;
                denominador = p1.mX - p0.mX;
                break;
            //Direita
            case 1:
                numerador = p0.mX - this->mXmax;
                denominador = p0.mX - p1.mX;
                break;
            //Superior
            case 2:
                numerador = p0.mY - this->mYmax;
                denominador = p0.mY - p1.mY;
                break;
            //Inferior
            case 3:
                numerador = this->mYmin - p0.mY;
                denominador = p1.mY - p0.mY;
                break;
        }
        // Se o segmento for paralelo ao lado de recorte
        if (denominador == 0){
            //Se o segmento estiver fora da área em relação ao lado
            if (numerador > 0 ){
                //Está fora da área de recorte, retorna segmento vazio
                return NULL;
            }
        } else {
            double t = numerador/denominador;
            // Se o segmento estiver potencialmente entrando
            if (denominador >= 0){
                // Se a interseção for posterior à posição fim
                if (t > fim){
                    return NULL;
                }
                // Se a interseção foi posterior à posição inicio
                if (t > inicio){
                    // Atualizar a posição de início
                    inicio = t;
                }
            // senão
            } else {
                // Se a interseção for anterior à posição de início, então
                if (t < inicio) {
                    return NULL;
                } 
                // Se a interseção for anterior à posição de fim, então
                if (t < fim ){
                    // Atualizar a posição de fim.
                    fim = t;
                }
            }
        }
    }
    // Calcular as coordenadas da posição de início, se não forem as de P0.
    if (inicio != 0){
        p0 = l.Coordinates(inicio);
    }
    // Calcular as coordenadas da posição de fim, se não forem as de P1.
    if(fim != 1){
        p1 = l.Coordinates(fim);
    }

    // Responder com um segmento dado pelas coordenadas calculadas anteriormente.
    return (new Line(p0,p1));
}

void ClipRectangle::Read(const string& prompt)
{
    cout << prompt;
    cin >> mXmin >> mXmax >> mYmin >> mYmax;
    if (not mInitialized)
        mInitialized = true;
}

ostream& operator << (ostream& os, const ClipRectangle& r)
{
    return os << "(Clipping Rectangle: " << r.mXmin << ", " << r.mXmax << ", " 
              << r.mYmin << ", " << r.mYmax << ")";
}



