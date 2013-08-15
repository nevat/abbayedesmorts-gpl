/* structs.h */
/* Almacena los structs utilizados en el juego */

/* Structs */
struct enem {
  int tipo[7];
  float x[7];
  float y[7];
  int direccion[7];
  int tilex[7];
  int tiley[7];
  int animacion[7];
  int limizq[7];
  int limder[7];
  int velocidad[7];
  int disparo[7];
	int ajustex1[7];
	int ajustex2[7];
	int ajustey1[7];
	int ajustey2[7];
};

struct protagonista {
	float x;
	float y;
	int direccion;
	int salto; /* 1-Saltando, 2-Cayendo */
	float altura; /* Contador para saber limite del salto */
	int animacion;
	float gravedad; /* Fuerza de la gravedad */
	int puntos[8]; /* Puntos de colision */
	int suelo; /* Pixel donde esta el suelo */
	int colision[4]; /* Colisiones, en 4 direcciones */
	int agachado;
	int checkpoint[4];
	int estado[2]; /* Vidas y cruces */
	int flags[7];
	int muerte;
	int pulsa[4]; /* Pulsaciones de teclas */
	int temp;
};
