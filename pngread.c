//
//  PNG画像の読み込みサンプル
//
#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>


void Display(void);
void Reshape(int, int);
void Timer(int);
void PutSprite(int, int, int, pngInfo *);


//  PNG画像を読み込むための変数
GLuint img1, img2;
pngInfo info1, info2;


int main(int argc, char **argv)
{
    //  一般的な準備
    srandom(12345);
    glutInit(&argc, argv);
    glutInitWindowSize(180, 180);
    glutCreateWindow("PNGファイル読み込みのテスト");
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
    glClearColor(0.0, 0.0, 1.0, 1.0);

    //  テクスチャのアルファチャネルを有効にする設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //  PNG画像の読み込み
    img1 = pngBind("acorn.png", PNG_NOMIPMAP, PNG_ALPHA, 
                   &info1, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    img2 = pngBind("pill.png", PNG_NOMIPMAP, PNG_ALPHA, 
                   &info2, GL_CLAMP, GL_NEAREST, GL_NEAREST);

    //  読み込んだPNG画像の情報を表示してみる
    printf("img1: id=%d, W=%d, H=%d, D=%d, A=%d\n", img1,
           info1.Width, info1.Height, info1.Depth, info1.Alpha);
    printf("img2: id=%d, W=%d, H=%d, D=%d, A=%d\n", img2,
           info2.Width, info2.Height, info2.Depth, info2.Alpha);

    //  コールバック関数の登録
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutTimerFunc(1000, Timer, 0);

    //  イベントループ突入
    glutMainLoop();

    return(0);
}


//
//  タイマー割り込みで画面を描画し直す
//
void Timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(1000, Timer, 0);
}


//
//  ウィンドウのサイズ変更が発生したときに座標系を再設定する関数
//
void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
}


//
//  ウィンドウの表示内容を更新する
//
void Display(void)
{
    int x, y;  //  PNG画像を置く座標
    int w, h;  //  現在のウィンドウのサイズ


    glClear(GL_COLOR_BUFFER_BIT);

    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);

    x = random() % w;
    y = random() % h;
    PutSprite(img1, x, y, &info1);

    x = random() % w;
    y = random() % h;
    PutSprite(img2, x, y, &info2);

    glFlush();
}


//
//  num番のPNG画像を座標(x,y)に表示する
//
void PutSprite(int num, int x, int y, pngInfo *info)
{
    int w, h;  //  テクスチャの幅と高さ


    w = info->Width;   //  テクスチャの幅と高さを取得する
    h = info->Height;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, num);
    glColor4ub(255, 255, 255, 255);

    glBegin(GL_QUADS);  //  幅w, 高さhの四角形

    glTexCoord2i(0, 0); 
    glVertex2i(x, y);

    glTexCoord2i(0, 1);
    glVertex2i(x, y + h);

    glTexCoord2i(1, 1);
    glVertex2i(x + w, y + h);

    glTexCoord2i(1, 0);
    glVertex2i(x + w, y);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
