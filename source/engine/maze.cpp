#include "headers/maze.h"
#include "opengl/resource_manager.h"
#include <time.h>
using namespace std;

mazeNode::mazeNode(int x, int y) {
    x_pos = x;
    y_pos = y;
    nbrs.insert({'N', false});
    nbrs.insert({'E', false});
    nbrs.insert({'S', false});
    nbrs.insert({'W', false});
}

mazeNode::mazeNode() {
}

dsuNode::dsuNode(int x_val, int y_val) {

    x = x_val;
    y = y_val;
    parent = NULL;
}

Maze::Maze(float x, float y, float width, float height, int r, int c) {
    START_X = x;
    START_Y = y;
    WIDTH = width;
    HEIGHT = height;

    rows = r;
    cols = c;
    box_width = WIDTH / cols;
    box_height = HEIGHT / rows;
    maze_grid.resize(rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mazeNode mNode(j, i);
            maze_grid[i].push_back(mNode);
        }
    }
    int middle_row = floor(rows / 2);
    int middle_col = floor(cols / 2);
    player_nodes.push_back(&maze_grid[middle_row][middle_col]);

    int imp_row = 1;
    int imp_col = 1;
    imposter_nodes.push_back(&maze_grid[imp_row][imp_col]);

    dist.resize(rows);
    for (int i = 0; i < rows; i++)
        dist[i].resize(cols);
}

dsuNode *Maze::find_root(dsuNode &n) {
    // cout << "finding roots of " << n.x << "," << n.y << " -> ";
    dsuNode *run = &n;
    while (run->parent != NULL) {
        //    cout << "going to parent " << run->x << "," << run->y << " > ";
        run = run->parent;
    }

    // cout << endl;

    return run;
}

void Maze::merge_nodes(dsuNode &n1, dsuNode &n2) {
    // cout << "merging " << n1.x << "," << n1.y << "  and  " << n2.x << "," << n2.y << endl;
    dsuNode *root_1 = find_root(n1);
    dsuNode *root_2 = find_root(n2);
    // cout << "roots are " << root_1->x << "," << root_1->y << "  and  " << root_2->x << "," << root_2->y << endl;

    root_2->parent = root_1;
}

void Maze::add_edge(int orig_x, int orig_y, char direction) {
    switch (direction) {
    case 'N':
        maze_grid[orig_y][orig_x].nbrs['N'] = true;
        // cout << "break N wall of " << orig_x << "," << orig_y << "  ";
        break;

    case 'E':
        maze_grid[orig_y][orig_x].nbrs['E'] = true;
        // cout << "break E wall of " << orig_x << "," << orig_y << "  ";
        break;

    case 'S':
        maze_grid[orig_y][orig_x].nbrs['S'] = true;
        // cout << "break S wall of " << orig_x << "," << orig_y << "  ";
        break;

    case 'W':
        maze_grid[orig_y][orig_x].nbrs['W'] = true;
        // cout << "break W wall of " << orig_x << "," << orig_y << "  ";
        break;
    }
}

void Maze::make_maze() {
    // dsu nodes
    vector<dsuNode> dsu_grid[rows];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dsuNode dNode(j, i);
            dsu_grid[i].push_back(dNode);
        }
    }

    //edges
    vector<pair<pair<unsigned int, unsigned int>, char>> edges;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int orig_x = dsu_grid[i][j].x;
            int orig_y = dsu_grid[i][j].y;

            if (orig_x < (cols - 1))
                edges.push_back({{orig_x, orig_y}, 'E'});

            if (orig_y < (rows - 1))
                edges.push_back({{orig_x, orig_y}, 'S'});
        }
    }

    //shuffling the edges around for random pattern
    unsigned int seed = time(NULL);
    shuffle(edges.begin(), edges.end(), default_random_engine(seed));

    int num_edges = 0;
    for (auto e : edges) {
        int o_x = e.first.first;
        int o_y = e.first.second;
        char direction = e.second;
        int dest_x;
        int dest_y;
        char back_direction;

        // getting the edge source node and dest node
        if (direction == 'E') {
            dest_x = dsu_grid[o_y][o_x + 1].x;
            dest_y = dsu_grid[o_y][o_x + 1].y;
            back_direction = 'W';
        } else {
            dest_x = dsu_grid[o_y + 1][o_x].x;
            dest_y = dsu_grid[o_y + 1][o_x].y;
            back_direction = 'N';
        }

        // cout << "((" << o_x << "," << o_y << ")" << " < > " << "(" << dest_x << "," << dest_y << "), " << direction << ")" << " ?? ";
        dsuNode *root_1 = find_root(dsu_grid[o_y][o_x]);
        dsuNode *root_2 = find_root(dsu_grid[dest_y][dest_x]);
        if (root_1 != root_2) {
            // cout << "adding edge between " << dsu_grid[o_y][o_x].x << "," << dsu_grid[o_y][o_x].y << " and " << dsu_grid[dest_y][dest_x].x << "," << dsu_grid[dest_y][dest_x].y << " = ";
            merge_nodes(dsu_grid[o_y][o_x], dsu_grid[dest_y][dest_x]);
            add_edge(o_x, o_y, direction);
            add_edge(dest_x, dest_y, back_direction);
            num_edges++;
        }

        // cout << endl;
    }

    // cout << num_edges << endl;
}

// function to make line vertices for drawing
void Maze::make_lines() {
    // maze boundaries
    line_vertices.push_back(START_X);
    line_vertices.push_back(START_Y);
    line_vertices.push_back(0.0);

    line_vertices.push_back(START_X + WIDTH);
    line_vertices.push_back(START_Y);
    line_vertices.push_back(0.0);

    line_vertices.push_back(START_X + WIDTH);
    line_vertices.push_back(START_Y);
    line_vertices.push_back(0.0);

    line_vertices.push_back(START_X + WIDTH);
    line_vertices.push_back(START_Y - HEIGHT);
    line_vertices.push_back(0.0);

    line_vertices.push_back(START_X + WIDTH);
    line_vertices.push_back(START_Y - HEIGHT);
    line_vertices.push_back(0.0);

    line_vertices.push_back(START_X);
    line_vertices.push_back(START_Y - HEIGHT);
    line_vertices.push_back(0.0);

    line_vertices.push_back(START_X);
    line_vertices.push_back(START_Y - HEIGHT);
    line_vertices.push_back(0.0);

    line_vertices.push_back(START_X);
    line_vertices.push_back(START_Y);
    line_vertices.push_back(0.0);

    // maze walls
    pair<float, float> top_left;
    pair<float, float> top_right;
    pair<float, float> bottom_right;
    pair<float, float> bottom_left;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mazeNode cur_node = maze_grid[i][j];
            float center_x = START_X + box_width / 2 + box_width * cur_node.x_pos;
            float center_y = START_Y - box_height / 2 - box_height * cur_node.y_pos;
            top_left = make_pair(center_x - box_width / 2, center_y + box_height / 2);
            top_right = make_pair(center_x + box_width / 2, center_y + box_height / 2);
            bottom_right = make_pair(center_x + box_width / 2, center_y - box_height / 2);
            bottom_left = make_pair(center_x - box_width / 2, center_y - box_height / 2);
            // cout << "at node" << cur_node.x_pos << "," << cur_node.y_pos << " - ";
            if (!cur_node.nbrs['N']) {
                // cout << "N" << " ";
                line_vertices.push_back(top_left.first);
                line_vertices.push_back(top_left.second);
                line_vertices.push_back(0.0);

                line_vertices.push_back(top_right.first);
                line_vertices.push_back(top_right.second);
                line_vertices.push_back(0.0);
            }

            if (!cur_node.nbrs['E']) {
                // cout << "E" << " ";
                line_vertices.push_back(top_right.first);
                line_vertices.push_back(top_right.second);
                line_vertices.push_back(0.0);

                line_vertices.push_back(bottom_right.first);
                line_vertices.push_back(bottom_right.second);
                line_vertices.push_back(0.0);
            }

            if (!cur_node.nbrs['S']) {
                // cout << "S" << " ";
                line_vertices.push_back(bottom_right.first);
                line_vertices.push_back(bottom_right.second);
                line_vertices.push_back(0.0);

                line_vertices.push_back(bottom_left.first);
                line_vertices.push_back(bottom_left.second);
                line_vertices.push_back(0.0);
            }

            if (!cur_node.nbrs['W']) {
                // cout << "W" << " ";
                line_vertices.push_back(bottom_left.first);
                line_vertices.push_back(bottom_left.second);
                line_vertices.push_back(0.0);

                line_vertices.push_back(top_left.first);
                line_vertices.push_back(top_left.second);
                line_vertices.push_back(0.0);
            }

            // cout << endl;
        }
    }
}

// drawing the exit
void Maze::draw_finish() {
    pair<float, float> finish_pos = find_pos(&(maze_grid[rows - 1][cols - 1]));
    float left = finish_pos.first - box_width / 2;
    float right = finish_pos.first + box_width / 2;
    float bottom = finish_pos.second - box_height / 2;
    float top = finish_pos.second + box_height / 2;

    // Setting up the structs and stuff
    struct VAO *vao_back = new struct VAO;
    vao_back->PrimitiveMode = GL_TRIANGLES;
    vao_back->NumVertices = 12;

    glGenVertexArrays(1, &(vao_back->VertexArrayID));
    glGenBuffers(1, &(vao_back->VertexBuffer));
    glGenBuffers(1, &(vao_back->ColorBuffer));

    glBindVertexArray(vao_back->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_back->VertexBuffer);

    float vertexBufferData_back[36] = {
        left,
        top,
        0.0f,
        (2 * left + right) / 3,
        top,
        0.0f,
        left,
        bottom,
        0.0f,

        left,
        bottom,
        0.0f,
        (2 * left + right) / 3,
        bottom,
        0.0f,
        (2 * left + right) / 3,
        top,
        0.0f,

        (2 * right + left) / 3,
        top,
        0.0f,
        right,
        top,
        0.0f,
        (2 * right + left) / 3,
        bottom,
        0.0f,

        (2 * right + left) / 3,
        bottom,
        0.0f,
        right,
        bottom,
        0.0f,
        right,
        top,
        0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData_back), vertexBufferData_back, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao_back->ColorBuffer);

    float back_color_buffer[36] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f};

    // cout << "hola" << endl;

    glBufferData(GL_ARRAY_BUFFER, sizeof(back_color_buffer), back_color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // draw it
    glBindVertexArray(vao_back->VertexArrayID);

    glDrawArrays(vao_back->PrimitiveMode, 0, vao_back->NumVertices);

    glBindVertexArray(0);
}

//drawing maze background , walls and finish
void Maze::draw_maze() {
    //BLACK BACKGROND
    // Setting up the structs and stuff
    struct VAO *vao_back = new struct VAO;
    vao_back->PrimitiveMode = GL_TRIANGLES;
    vao_back->NumVertices = 6;

    glGenVertexArrays(1, &(vao_back->VertexArrayID));
    glGenBuffers(1, &(vao_back->VertexBuffer));
    glGenBuffers(1, &(vao_back->ColorBuffer));

    glBindVertexArray(vao_back->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_back->VertexBuffer);

    float vertexBufferData_back[18] = {
        START_X, START_Y, 0.0f,
        START_X + WIDTH, START_Y, 0.0f,
        START_X, START_Y - HEIGHT, 0.0f,

        START_X, START_Y - HEIGHT, 0.0f,
        START_X + WIDTH, START_Y - HEIGHT, 0.0f,
        START_X + WIDTH, START_Y, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData_back), vertexBufferData_back, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao_back->ColorBuffer);

    float back_color_buffer[18] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(back_color_buffer), back_color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // draw it
    glBindVertexArray(vao_back->VertexArrayID);

    glDrawArrays(vao_back->PrimitiveMode, 0, vao_back->NumVertices);

    glBindVertexArray(0);

    this->draw_finish();

    //LINES
    // Setting up the structs and stuff
    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_LINES;
    unsigned int numVerts = line_vertices.size();
    numVerts = numVerts / 3;
    vao->NumVertices = numVerts;

    glGenVertexArrays(1, &(vao->VertexArrayID));
    glGenBuffers(1, &(vao->VertexBuffer));
    glGenBuffers(1, &(vao->ColorBuffer));

    glBindVertexArray(vao->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    float vertexBufferData[line_vertices.size()];

    for (int i = 0; i < line_vertices.size(); i++)
        vertexBufferData[i] = line_vertices[i];

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    float color_buffer[line_vertices.size()];
    for (int i = 0; i < line_vertices.size(); i++) {
        if (i % 3 == 0)
            color_buffer[i] = 1.0f;
        else if (i % 3 == 1)
            color_buffer[i] = 1.0f;
        else
            color_buffer[i] = 1.0f;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer), color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // draw it
    glBindVertexArray(vao->VertexArrayID);

    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices);

    glBindVertexArray(0);
}

int Maze::vertex_dist(float x, float y) {
    int m = INF;
    mazeNode *nbr_node;
    //top left
    nbr_node = find_node(x - box_width / 2, y + box_height / 2);
    if (nbr_node != NULL) {
        if (dist[nbr_node->y_pos][nbr_node->x_pos] < m)
            m = dist[nbr_node->y_pos][nbr_node->x_pos];
    }

    //top right
    nbr_node = find_node(x + box_width / 2, y + box_height / 2);
    if (nbr_node != NULL) {
        if (dist[nbr_node->y_pos][nbr_node->x_pos] < m)
            m = dist[nbr_node->y_pos][nbr_node->x_pos];
    }

    //bottom right
    nbr_node = find_node(x + box_width / 2, y - box_height / 2);
    if (nbr_node != NULL) {
        if (dist[nbr_node->y_pos][nbr_node->x_pos] < m)
            m = dist[nbr_node->y_pos][nbr_node->x_pos];
    }

    //bottom left
    nbr_node = find_node(x - box_width / 2, y - box_height / 2);
    if (nbr_node != NULL) {
        if (dist[nbr_node->y_pos][nbr_node->x_pos] < m)
            m = dist[nbr_node->y_pos][nbr_node->x_pos];
    }

    return m;
}

void Maze::draw_light() {
    //BLACK BACKGROND
    // Setting up the structs and stuff
    struct VAO *vao_back = new struct VAO;
    vao_back->PrimitiveMode = GL_TRIANGLES;
    vao_back->NumVertices = 6;

    glGenVertexArrays(1, &(vao_back->VertexArrayID));
    glGenBuffers(1, &(vao_back->VertexBuffer));
    glGenBuffers(1, &(vao_back->ColorBuffer));

    glBindVertexArray(vao_back->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_back->VertexBuffer);

    float vertexBufferData_back[18] = {
        START_X, START_Y, 0.0f,
        START_X + WIDTH, START_Y, 0.0f,
        START_X, START_Y - HEIGHT, 0.0f,

        START_X, START_Y - HEIGHT, 0.0f,
        START_X + WIDTH, START_Y - HEIGHT, 0.0f,
        START_X + WIDTH, START_Y, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData_back), vertexBufferData_back, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao_back->ColorBuffer);

    float back_color_buffer[18] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f};

    // cout << "hola" << endl;

    glBufferData(GL_ARRAY_BUFFER, sizeof(back_color_buffer), back_color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // draw it
    glBindVertexArray(vao_back->VertexArrayID);

    glDrawArrays(vao_back->PrimitiveMode, 0, vao_back->NumVertices);

    glBindVertexArray(0);

    this->draw_finish();

    //LINES
    // Setting up the structs and stuff
    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_LINES;
    unsigned int numVerts = line_vertices.size();
    numVerts = numVerts / 3;
    vao->NumVertices = numVerts;

    glGenVertexArrays(1, &(vao->VertexArrayID));
    glGenBuffers(1, &(vao->VertexBuffer));
    glGenBuffers(1, &(vao->ColorBuffer));

    glBindVertexArray(vao->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    float vertexBufferData[line_vertices.size()];

    for (int i = 0; i < line_vertices.size(); i++)
        vertexBufferData[i] = line_vertices[i];

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    float color_buffer[line_vertices.size()];
    float reduction_factor;
    int dist;
    for (int i = 0; i < (vao->NumVertices); i++) {
        dist = vertex_dist(vertexBufferData[3 * i], vertexBufferData[3 * i + 1]);
        if (dist > 4)
            reduction_factor = 0;
        else
            reduction_factor = 1 / (float)pow(3, (dist));

        color_buffer[3 * i] = 1.0f * reduction_factor;

        color_buffer[3 * i + 1] = 1.0f * reduction_factor;

        color_buffer[3 * i + 2] = 1.0f * reduction_factor;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer), color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // draw it
    glBindVertexArray(vao->VertexArrayID);

    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices);

    glBindVertexArray(0);
}

//FUNCTIONS TO DRAW MAZE OBJECTS

void Maze::draw_tasks() {
    if (this->task1->exist)
        this->task1->draw();
    if (this->task2->exist)
        this->task2->draw();
}

void Maze::draw_tasks_light() {
    if (this->task1->exist)
        this->task1->draw_light(dist[this->task1->node_y][this->task1->node_x]);
    if (this->task2->exist)
        this->task2->draw_light(dist[this->task2->node_y][this->task2->node_x]);
}

void Maze::draw_buttons() {
    this->imposter_button->draw();
    this->powerup_button->draw();
}

void Maze::draw_buttons_light() {
    this->imposter_button->draw_light(this->dist[this->imposter_button->node_y][this->imposter_button->node_x]);
    this->powerup_button->draw_light(this->dist[this->powerup_button->node_y][this->powerup_button->node_x]);
}

void Maze::draw_powerups() {
    for (auto pUp : this->powerups) {
        if (pUp->exist)
            pUp->draw();
    }
}

void Maze::draw_obstacles() {
    for (auto obs : this->obstacles) {
        if (obs->exist)
            obs->draw();
    }
}

void Maze::draw_powerups_light() {
    for (auto pUp : this->powerups) {
        if (pUp->exist)
            pUp->draw_light(this->dist[pUp->node_y][pUp->node_x]);
    }
}

void Maze::draw_obstacles_light() {
    for (auto obs : this->obstacles) {
        if (obs->exist)
            obs->draw_light(this->dist[obs->node_y][obs->node_x]);
    }
}

pair<float, float> Maze::find_pos(mazeNode *mNode) {
    pair<float, float> coords;
    float x_coord = START_X + box_width / 2 + (mNode->x_pos) * (box_width);
    float y_coord = START_Y - box_height / 2 - (mNode->y_pos) * (box_height);
    coords.first = x_coord;
    coords.second = y_coord;

    return coords;
}

mazeNode *Maze::find_node(float x, float y) {
    x = x - START_X;
    y = START_Y - y;
    if (x < 0.0f || y < 0.0f || x > WIDTH || y > HEIGHT)
        return NULL;

    int x_coord = floor(x / box_width);
    int y_coord = floor(y / box_height);

    return &maze_grid[y_coord][x_coord];
}

vector<mazeNode *> Maze::find_player(hitbox_t player_hitbox) {
    mazeNode *cur_node;
    hitbox_t node_hitbox;
    pair<float, float> cur_coords;
    vector<mazeNode *> activated_nodes;
    bool isIntersecting;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cur_node = &maze_grid[i][j];
            cur_coords = find_pos(cur_node);
            node_hitbox = get_hitbox(cur_coords.first, cur_coords.second, box_width, box_width);
            isIntersecting = detect_collision(player_hitbox, node_hitbox);
            if (isIntersecting)
                activated_nodes.push_back(cur_node);
        }
    }

    return activated_nodes;
}

void Maze::update_player(hitbox_t player_hitbox) {
    player_nodes.clear();
    vector<mazeNode *> active_nodes = find_player(player_hitbox);
    for (auto av : active_nodes)
        player_nodes.push_back(av);
}

vector<mazeNode *> Maze::find_imposter(hitbox_t imposter_hitbox) {
    mazeNode *cur_node;
    hitbox_t node_hitbox;
    pair<float, float> cur_coords;
    vector<mazeNode *> activated_nodes;
    bool isIntersecting;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cur_node = &maze_grid[i][j];
            cur_coords = find_pos(cur_node);
            node_hitbox = get_hitbox(cur_coords.first, cur_coords.second, box_width, box_width);
            isIntersecting = detect_collision(imposter_hitbox, node_hitbox);
            if (isIntersecting)
                activated_nodes.push_back(cur_node);
        }
    }

    return activated_nodes;
}

void Maze::update_imposter(hitbox_t imposter_hitbox) {
    imposter_nodes.clear();
    vector<mazeNode *> active_nodes = find_imposter(imposter_hitbox);
    for (auto av : active_nodes)
        imposter_nodes.push_back(av);
}

bool Maze::check_collision(hitbox_t possible_hitbox, vector<mazeNode *> activated_nodes) {
    vector<hitbox_t> possible_walls;
    pair<float, float> node_coords;
    for (auto av : activated_nodes) {
        node_coords = find_pos(av);
        if (!(av->nbrs['N']))
            possible_walls.push_back(get_hitbox(node_coords.first, node_coords.second + (box_height / 2), box_width, 0.0f));

        if (!(av->nbrs['E']))
            possible_walls.push_back(get_hitbox(node_coords.first + (box_width / 2), node_coords.second, 0.0f, box_height));

        if (!(av->nbrs['S']))
            possible_walls.push_back(get_hitbox(node_coords.first, node_coords.second - (box_height / 2), box_width, 0.0f));

        if (!(av->nbrs['W']))
            possible_walls.push_back(get_hitbox(node_coords.first - (box_width / 2), node_coords.second, 0.0f, box_height));
    }

    bool collided = false;

    for (auto poss_wall : possible_walls) {
        collided = detect_collision(possible_hitbox, poss_wall);
        if (collided)
            break;
    }

    return collided;
}

void Maze::bfs() {
    bool visited[rows][cols];
    for (int i = 0; i < rows; i++) {
        dist[i].clear();
        dist[i].resize(cols);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            visited[i][j] = false;
        }
    }

    queue<mazeNode *> q;
    for (auto pNodes : player_nodes) {
        visited[pNodes->y_pos][pNodes->x_pos] = true;
        dist[pNodes->y_pos][pNodes->x_pos] = 0;
        q.push(pNodes);
    }

    mazeNode *nbr;
    char cur_dir;
    bool found = false;
    while (!q.empty()) {
        mazeNode *cur_node = q.front();
        // cout << "at node " << cur_node->x_pos << "," << cur_node->y_pos << "  ";
        q.pop();
        if (cur_node->nbrs['N']) {

            nbr = &maze_grid[cur_node->y_pos - 1][cur_node->x_pos];
            if (!(visited[nbr->y_pos][nbr->x_pos])) {
                // cout << "up" << " ";
                visited[nbr->y_pos][nbr->x_pos] = true;
                dist[nbr->y_pos][nbr->x_pos] = dist[cur_node->y_pos][cur_node->x_pos] + 1;
                q.push(nbr);
                if (!found) {
                    for (auto iNode : imposter_nodes) {
                        if (nbr->x_pos == iNode->x_pos && nbr->y_pos == iNode->y_pos) {
                            // cout << "- at imposter node" << " ";
                            cur_dir = 'S';
                            found = true;
                        }
                    }
                }
            }
        }

        if (cur_node->nbrs['E']) {

            nbr = &maze_grid[cur_node->y_pos][cur_node->x_pos + 1];
            if (!(visited[nbr->y_pos][nbr->x_pos])) {
                // cout << "right" << " ";
                visited[nbr->y_pos][nbr->x_pos] = true;
                dist[nbr->y_pos][nbr->x_pos] = dist[cur_node->y_pos][cur_node->x_pos] + 1;
                q.push(nbr);
                if (!found) {
                    for (auto iNode : imposter_nodes) {
                        if (nbr->x_pos == iNode->x_pos && nbr->y_pos == iNode->y_pos) {
                            // cout << "- at imposter node" << " ";
                            cur_dir = 'W';
                            found = true;
                        }
                    }
                }
            }
        }

        if (cur_node->nbrs['S']) {

            nbr = &maze_grid[cur_node->y_pos + 1][cur_node->x_pos];
            if (!(visited[nbr->y_pos][nbr->x_pos])) {
                // cout << "down" << " ";
                visited[nbr->y_pos][nbr->x_pos] = true;
                dist[nbr->y_pos][nbr->x_pos] = dist[cur_node->y_pos][cur_node->x_pos] + 1;
                q.push(nbr);
                if (!found) {
                    for (auto iNode : imposter_nodes) {
                        if (nbr->x_pos == iNode->x_pos && nbr->y_pos == iNode->y_pos) {
                            // cout << "- at imposter node" << " ";
                            cur_dir = 'N';
                            found = true;
                        }
                    }
                }
            }
        }

        if (cur_node->nbrs['W']) {

            nbr = &maze_grid[cur_node->y_pos][cur_node->x_pos - 1];
            if (!(visited[nbr->y_pos][nbr->x_pos])) {
                // cout << "left" << " ";
                visited[nbr->y_pos][nbr->x_pos] = true;
                dist[nbr->y_pos][nbr->x_pos] = dist[cur_node->y_pos][cur_node->x_pos] + 1;
                q.push(nbr);
                if (!found) {
                    for (auto iNode : imposter_nodes) {
                        if (nbr->x_pos == iNode->x_pos && nbr->y_pos == iNode->y_pos) {
                            // cout << "- at imposter node" << " ";
                            cur_dir = 'E';
                            found = true;
                        }
                    }
                }
            }
        }
    }

    // cout << endl;

    if (imposter_nodes.size() > 0) {
        if (imposter_nodes.size() == 1) {
            this->imposter_move = cur_dir;
        } else {
            mazeNode *dest_node;
            mazeNode *other_node;
            if (dist[imposter_nodes[0]->y_pos][imposter_nodes[0]->x_pos] < dist[imposter_nodes[1]->y_pos][imposter_nodes[1]->x_pos]) {
                dest_node = imposter_nodes[0];
                other_node = imposter_nodes[1];
            } else {
                dest_node = imposter_nodes[1];
                other_node = imposter_nodes[0];
            }

            if (dest_node->x_pos == other_node->x_pos) {
                if (dest_node->y_pos < other_node->y_pos)
                    this->imposter_move = 'N';
                else
                    this->imposter_move = 'S';
            } else if (dest_node->y_pos == other_node->y_pos) {
                if (dest_node->x_pos < other_node->x_pos)
                    this->imposter_move = 'W';
                else
                    this->imposter_move = 'E';
            }
        }
    }
}

bool Maze::check_task(hitbox_t player_hitbox) {
    if (this->task1->exist && detect_collision(player_hitbox, get_hitbox(this->task1->position.x, this->task1->position.y, this->task1->width, this->task1->height))) {

        this->task1->exist = false;
        return true;

    } else if (this->task2->exist && detect_collision(player_hitbox, get_hitbox(this->task2->position.x, this->task2->position.y, this->task2->width, this->task2->height))) {
        this->task2->exist = false;
        return true;
    }

    return false;
}

bool Maze::check_imposter_button(hitbox_t player_hitbox) {
    if (!this->imposter_button->pressed && detect_collision(player_hitbox, get_hitbox(this->imposter_button->position.x, this->imposter_button->position.y, this->imposter_button->radius, this->imposter_button->radius))) {
        this->imposter_button->pressed = true;
        this->imposter_button->color = COLOR_WHITE;
        return true;
    }

    return false;
}

bool Maze::check_powerup_button(hitbox_t player_hitbox) {
    if (!this->powerup_button->pressed && detect_collision(player_hitbox, get_hitbox(this->powerup_button->position.x, this->powerup_button->position.y, this->powerup_button->radius, this->powerup_button->radius))) {
        this->powerup_button->pressed = true;
        this->powerup_button->color = COLOR_WHITE;
        return true;
    }

    return false;
}

bool Maze::check_powerup(hitbox_t player_hibox) {
    for (auto pUp : this->powerups) {
        if (pUp->exist && detect_collision(player_hibox, get_hitbox(pUp->position.x, pUp->position.y, pUp->width, pUp->height))) {
            pUp->exist = false;
            return true;
        }
    }

    return false;
}

void Maze::spawn_powerups() {
    for (auto pUp : this->powerups) {
        pUp->exist = true;
    }
}

bool Maze::check_obstacle(hitbox_t player_hibox) {
    for (auto obs : this->obstacles) {
        if (obs->exist && detect_collision(player_hibox, get_hitbox(obs->position.x, obs->position.y, obs->width, obs->height))) {
            return true;
        }
    }

    return false;
}

void Maze::spawn_obstacles() {
    for (auto obs : this->obstacles) {
        obs->exist = true;
    }
}

bool Maze::check_kill() {
    bool killed = false;
    for (auto pNode : player_nodes) {
        for (auto iNode : imposter_nodes) {
            if (pNode->x_pos == iNode->x_pos && pNode->y_pos == iNode->y_pos) {
                killed = true;
                break;
            }
        }
    }

    return killed;
}

bool Maze::check_finish() {
    bool finished = false;
    for (auto pNode : player_nodes) {
        if (pNode->x_pos == cols - 1 && pNode->y_pos == rows - 1) {
            finished = true;
            break;
        }
    }

    return finished;
}
