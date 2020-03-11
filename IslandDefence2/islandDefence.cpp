/****************************************************************************
* Ben Bartley, Keyin Lee
* s3646117, s3583015
* May 2018
* islandDefence.cpp
***************************************************************************/

#include "islandDefence.h"

// Global time
Global global = { false, 0, 0, 0, 0, true, 0, 0, 1, 0, 0, 5.0f };

// Sine waves
//Sinewave waveA = { 0.25f, 2 * (float)M_PI, 1 };
Sinewave waveA = { 0.05f, 2 * (float)M_PI, 0.25 * (float)M_PI };
Sinewave waveB = { 0.125f, (float)M_PI, 0.5 * (float)M_PI };

// Water wave
Wave wave = { 64, waveA, waveB };

// Toggle wireframe
bool isWireframe = false;

// Tracks game over state
bool isGameOver = false;

// Show/hide tangents, normals and binormals
bool showTangents = false;
bool showNormals = false;
bool showBinormals = false;

typedef struct {
    bool wireframeMode;
    bool texture;
    bool lighting;
    bool drawAxes;
    int x, y;
    int lastX, lastY;
    int rotate;
    int zoom;
    float scale;
    int direction;
    float angleY;
    float angleX;
	
} Camera;

Camera camera = { false, false, false, true, 0, 0, 0, 0, 0, 0, 1.0f, 1, 0.0, 0.0 };

// Manages quadric objects
GLUquadric* qobj;

// Values for boats
float bBoatVals[n_joints] = { -0.5f, 30.0f };
float bBoatIncs[n_joints] = { 0.01f, 1.0f };

// Distance boats spawn from island
float spawnDistance = 2.5f;

// Create island
Island island = { 100, 100, 0, 1, 0, 0, -0.375f, 1.5f, 1.0f, 0.06f, 0.25f, 0.0f, 0, 1, 1, 1 };

const float g = -2.0f;              // Gravity
const float missileForce = 1.75f;    // Speed of missile
const float defenceForce = 2.0f;    // Speed of defence projectile

// Create vectors to track all missiles/defences and boats
std::vector<Missile> missiles;
std::vector<Defence> defences;                           
std::vector<Boat> boats;

static GLuint texture;
#if 0
static GLuint loadTexture(const char *filename)
{
    GLuint tex = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    if (!tex)
        return 0;

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}
#endif

// Update boats
void updateBoatState(float dt)
{
    for (auto it = boats.begin(); it != boats.end();)
    {
        // Move towards island
        it->move(global);
        // Fire if possible
        if (it->fireTimer > it->fireDelay)
        {
            it->fireMissile(missiles, missileForce, wave, global);
            it->fireTimer = 0;
        }
        // Destroy if no health remaining
        if(it->health < 1)
            it = boats.erase(it);
        else
            ++it;
    }
}

// Update missile projectiles
void updateMissileState(float dt)
{
    for (auto &m : missiles)
    {
        // Position
        m.pos.x += m.v.x * dt;
        m.pos.y += m.v.y * dt;
        m.pos.z += m.v.z * dt;

        // Velocity
        m.v.y += g * dt;
    }
}

// Update defence projectiles
void updateDefenceState(float dt)
{
    for (auto &d : defences)
    {
        // Position
        d.pos.x += d.v.x * dt;
        d.pos.y += d.v.y * dt;
        d.pos.z += d.v.z * dt;

        // Velocity
        d.v.y += g * d.gravMultiplier * dt;

        // Scale
        d.radius += d.increment * dt;

    }
}

// Update state for all projectiles
void updateProjectileState(float dt)
{
    updateMissileState(dt);
    updateDefenceState(dt);
}

// Delete defence projectile after a period of time
void handleDefenceTimeout()
{
    for (auto it = defences.begin(); it != defences.end();)
    {
        if (it->lingerTimer > it->lingerDelay)
            it = defences.erase(it);
        else
            ++it;
    }
}

// Draw health bars
void drawHealthBars()
{
    float barWidth = 0.5f;
    float barHeight = 0.05f; 

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set up orthographic coordinate system to match the window   
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glScalef(0.75f, 1, 1);
    glTranslatef(0.1f, 0.9f, 0);
    island.drawHealthBar(barWidth, barHeight);
    
    glMatrixMode(GL_PROJECTION);

    // Pop projection 
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
}

// Handles boat
void handleBoatCollisions()
{
    for (auto it = boats.begin(); it != boats.end();)
    {
        // Check collision with island
        if (it->checkCollision(island, wave, global))
        {
            --island.health;
            it = boats.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Handles missile collisions
void handleMissileCollisions()
{
    for (auto it = missiles.begin(); it != missiles.end();)
    {
        // Check collision with boats
        if (it->checkCollision(boats, wave, global)) 
        {        
            it = missiles.erase(it);
        }
        // Check collision with island
        else if (it->checkCollision(island, wave, global))
        {
            --island.health;
            it = missiles.erase(it);
        }
        // Check collision with defence projectile and sine wave
        else if (it->checkCollision(defences, wave, global))
        {
            it = missiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Handles defence projectile collisions
void handleDefenceCollisions()
{
    for (auto it = defences.begin(); it != defences.end();)
    {
        // Check collision with island
        if (it->checkCollision(island, wave, global))
        {
            it = defences.erase(it);
        }
        // Check collision with sine wave
        else if (it->checkCollision(wave, global))
        {
            it = defences.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Check for projectile collisions
void handleCollisions()
{
    handleMissileCollisions();
    handleDefenceCollisions();
    handleBoatCollisions();
}

// Check for game over state
void handleGameOver()
{
    if (island.health == 0)
        isGameOver = true;

    if (isGameOver)
    {
        // Display last frame with game over message
        glutPostRedisplay();
    }
}

// Increment various timers
void updateTimers(float dt)
{
    island.fireTimer += dt;
    global.spawnBoatTimer += dt;
    for (auto &d : defences)
        d.lingerTimer += dt;
    for (auto &b : boats)
        b.fireTimer += dt;
}

// Idle callback for animation
void update()
{
    static float lastT = -1.0;

    glutPostRedisplay();
    
    // Only update if game is not over
    if (!global.isActive || isGameOver)
        return;
    
    // Update time
    global.t = glutGet(GLUT_ELAPSED_TIME) / (float)milli - global.startTime + global.pauseT;

    if (lastT < 0.0) {
        lastT = global.t;
        return;
    }

    // Calculate change in time since last call
    global.dt = global.t - lastT;

    // Update projectiles and timers
    updateProjectileState(global.dt);
    updateTimers(global.dt);

    // Spawn boat if enough time has passed
    if (global.spawnBoatTimer > global.spawnBoatDelay)
    {
        // Instantiate boat
        Boat boat = { 1, 10, 1, 0, 0, bBoatVals, bBoatIncs, 0.1f, 0, 0, 0.5f, 2, 1, qobj, 1.25f, 0.25f, 1, 3, { -0.5f, 0.0f, 0.0f }, 0.05f };
        
        // Position boat correctly
        boat.positionBoat(spawnDistance);

        // Add boat to vector for iteration
        boats.push_back(boat);

        // Reset spawn delay
        global.spawnBoatTimer = 0;
    }

    // Update boat state
    updateBoatState(global.dt);

    // Check various conditions
    handleCollisions(); 
    handleDefenceTimeout();
    handleGameOver();
    
    lastT = global.t;

    // Calculate frame rate
    float dt = global.t - global.lastFrameRateT;
    if (dt > global.frameRateInterval) 
    {
        global.frameRate = global.frames / dt;
        global.lastFrameRateT = global.t;
        global.frames = 0;
    }
}

// On-screen display (text)
void displayOSD()
{
    char buffer[30];
    char *bufp;
    int w, h;
    int offsetW, offsetH;
    int spacing = 20;
    
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set up orthographic coordinate system to match the window   
    w = glutGet(GLUT_WINDOW_WIDTH);         // window width
    h = glutGet(GLUT_WINDOW_HEIGHT);        // window height
    offsetW = w - 170;
    offsetH = h - 30;
    glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Display frame rate text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(offsetW, offsetH);
    std::snprintf(buffer, sizeof buffer, "fr (f/s): %6.0f", global.frameRate);
    for (bufp = buffer; *bufp; bufp++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

    // Display time per frame text 
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(offsetW, offsetH - spacing);
    std::snprintf(buffer, sizeof buffer, "ft (ms/f): %5.0f", 1.0 / global.frameRate * milli);
    for (bufp = buffer; *bufp; bufp++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

    // Display tesselation text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(offsetW, offsetH - spacing * 2);
    std::snprintf(buffer, sizeof buffer, "tess: %10.0i", wave.segments);
    for (bufp = buffer; *bufp; bufp++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

    // Display game over text
    if (isGameOver)
    {
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2i(30, offsetH);
	std::snprintf(buffer, sizeof buffer, "Game Over");
        for (bufp = buffer; *bufp; bufp++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    }

    drawHealthBars();
    
    // Pop modelview
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);

    // Pop projection 
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // Pop attributes
    glPopAttrib();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    if(camera.wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Reset
    glLoadIdentity();
    // Position scene in view
    glTranslatef(0, -0.80f, 1.0f);
    glRotatef(-20.0f, 1.0f, 0, 0);
    
    if (camera.lighting) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    
    if (camera.texture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else
        glDisable(GL_TEXTURE_2D);
    
    glRotatef(camera.angleX, 1, 0, 0);
    glRotatef(camera.angleY, 0, 1, 0);
    glScalef(camera.scale, camera.scale, camera.scale);
    
    // Show/hide water tangents
    if (showTangents)
        wave.drawWaterTangents(global);

    // Show/hide water normals
    if (showNormals)
        wave.drawWaterNormals(global);

    // Show/hide water binormals
    if (showBinormals)
        wave.drawWaterBinormals(global);

    // Draw health bars for boats/island
    drawHealthBars();
    
    // Display OSD
    if (global.OSD)
        displayOSD();

    // Draw remaining objects
    island.drawIsland();
    island.drawCannon();

    // Draw boats
    for (auto &b : boats)
    {
        b.drawBoat(wave, global);
    }
    
    for (auto &m : missiles)
    {
        m.drawMissile();
        m.drawTrajectory(wave, global, g);
    }

    for (auto &d : defences)
    {
        d.drawDefence();
    }
   
    drawAxes();
    
    // Draw wave last for alpha blending
    wave.drawWater(global);

    glutSwapBuffers();

    global.frames++;
}

// Keyboard input
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:                                // Exit program
    case 'q':
        exit(EXIT_SUCCESS);
        break;
    case 'p':                               // Wireframe mode    
	    camera.wireframeMode =!camera.wireframeMode;
        break;
    case 'h':                              // Start/stop
        if (!global.isActive) 
        {
            global.startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
            global.isActive = true;
        }
        else
        {
            global.pauseT = global.t;        // Allows animation to 'resume'
            global.isActive = false;
        }
        break;
    case '+':                        // Double number of water segments
        wave.segments *= 2;      
        break;
    case '-':                       // Halve number of water segments
        if (wave.segments > 4)
            wave.segments /= 2;  
        break;
    case 'n':                       // Display/hide normals
        showNormals = !showNormals;
        showTangents = !showTangents;
        showBinormals = !showBinormals;
        break;
    case 32:                       // Fire missile island
        if (island.fireTimer > island.fireDelay && !isGameOver)
        {
            island.fireMissile(missiles, missileForce);
            island.fireTimer = 0;
        }          
        break;
    case 'v':                       // Fire defence island
        if (island.fireTimer > island.fireDelay && !isGameOver)
        {
            island.fireDefence(defences, defenceForce);
            island.fireTimer = 0;
        }
        break;
    case 'l':
        camera.lighting = !camera.lighting;
        break;
    case 'd':
        if (camera.angleX < 15.0) {
            camera.angleX += 0.5;
            island.turretRot -= 0.5;
        }
        break;

    case 'a':
        if (camera.angleX > -15.0) {
            camera.angleX -= 0.5;
            island.turretRot += 0.5;
        }
        break;
    case 't':
        camera.texture = !camera.texture;
        break;
    default:
        break;
    }
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        camera.angleY += 3.0;
        island.baseRot -= 3.0;
        break;
    case GLUT_KEY_RIGHT:
        camera.angleY -= 3.0;
        island.baseRot += 3.0;
        break;
    case GLUT_KEY_UP:
        camera.scale += camera.scale * 0.01;
        break;
    case GLUT_KEY_DOWN:
        camera.scale -= camera.scale * 0.01;
        break;
    }
}

// Initialise display
void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();			// Overwrite the contents with the identity
    gluPerspective(75, glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.01, 100);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Seed rand
    srand(static_cast <unsigned> (time(0)));
    
    // Initialise quadric
    qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    //set light position
    GLfloat light_position[] = { 0.5, 0.5, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

#if 0
    //set material properties
    GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat brown[] = { 0.0, 0.5, 0.5, 1.0 };
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_shininess[] = { 60.0 };
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, brown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, light_shininess);
#endif
}

// Main loop
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Island Defence");

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Keyboard functionality
    glutKeyboardFunc(keyboard);  
    glutSpecialFunc(specialKeyboard);
    
    // Display scene
    glutDisplayFunc(display);    
    
    // Update state
    glutIdleFunc(update);
    
    init();

#if 0
    texture = loadTexture("pink.jpg");
    if (!texture)
    {
        printf("No texture created; exiting.\n");
        return EXIT_FAILURE;
    }
#endif

    glutMainLoop();

    return EXIT_SUCCESS;
}
