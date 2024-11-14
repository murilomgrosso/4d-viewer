/* 
 * FUNCOES PARA GERENCIAMENTO DE MOUSE E TECLADO
 */

#ifndef INPUT_H
#define INPUT_H

#define MAX_MOUSE_WHEEL_BUFFER 16

/*----------------------------------------------------------------------------------------*/
// DECLARACAO DE FUNCOES

namespace input{
    static bool pressedMouse[3];            // Lista de botoes do mouse pressionados 
    static bool pressedKeys[255];           // Lista de teclas ASCII pressionadas
    static bool pressedSpecialKeys[255];    // Lista de teclas especiais GLUT pressionadas   
   
    /*
     * Lista circular de momentos scroll. Modulo dos valores representa o momento da acao.
     * Sinal dos valores representa se foi um scroll-up (+) ou scroll-down (-).
     */
    static double scrollTimestamps[MAX_MOUSE_WHEEL_BUFFER] = {0};
    static int lastScrollIndex = 0;         // Inicio da lista de scroll 

    void specialKeysUp(int key, int x, int y);              	// Gatilho para tecla especial liberada
    void specialKeysDown(int key, int x, int y);            	// Gatilho para tecla especial pressionada
    void keyboardUp(unsigned char key, int x, int y);       	// Gatilho para tecla liberada
    void keyboardDown(unsigned char key, int x, int y);     	// Gatilho para tecla pressionada
    void mouseManager(int button, int state, int x, int y);     // Gatilho para botoes do mouse

    bool isSpecialKeyPressed(int key);          // Verifica se a tecla especial GLUT foi pressionada
    bool isKeyPressed(unsigned char key);       // Verifica se a tecla ASCII foi pressionada
    bool isMouseButtonPressed(int button);      // Verifica se botao do mouse foi pressionado

    bool isUpPressed();             // Verifica se W/UP foi pressionada
    bool isDownPressed();           // Verifica se S/DOWN foi pressionada
    bool isLeftPressed();           // Verifica se A/LEFT foi pressionada
    bool isRightPressed();          // Verifica se D/RIGHT foi pressionada

    short getVerticalAxis();      	            // Retorna a direcao vertical (-1, 0 ou 1)
    short getHorizontalAxis();                  // Retorna a direcao horizontal (-1, 0 ou 1)
    short getMouseButtonAxis();                 // Retorna a direcao do botao do mouse (-1, 0 ou 1)
    short getMouseWheel(double thresholdTime);   // Retorna a direcao da wheel do mouse (-1, 0 ou 1)
}

#endif
