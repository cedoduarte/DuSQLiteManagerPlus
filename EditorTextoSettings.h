#ifndef EDITORTEXTOSETTINGS_H
#define EDITORTEXTOSETTINGS_H

#include <QColor>
#include <QFont>

class EditorTextoSettings
{
public:
    EditorTextoSettings(const QFont &fuente = QFont(), int tamanyo = -1,
                        const QColor &color = QColor());
    void setFuente(const QFont &fuente) { mFuente = fuente; }
    void setTamanyo(int tamanyo) { mTamanyo = tamanyo; }
    void setColor(const QColor &color) { mColor = color; }
    QFont getFuente() const { return mFuente; }
    int getTamanyo() const { return mTamanyo; }
    QColor getColor() const { return mColor; }
private:
    QFont mFuente;
    int mTamanyo;
    QColor mColor;
};

#endif // EDITORTEXTOSETTINGS_H
