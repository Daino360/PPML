#include <stdio.h>
#include <stdlib.h>	
#include <time.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "Particle.h"
#include "MazeGenerator.h"
#include <algorithm>
#include <chrono>
#include <omp.h>


Particle::Particle(vector<int> particlePos, MazeGenerator maze){
    maze1 = maze.getMaze();    //initilize the maze
    move(maze);                //move the particle
}

void Particle::move(MazeGenerator maze){
    int direction, numThreads, threadID; 
    vector<int> particleTmp = particle;                    //particle position
    //auto start = chrono::high_resolution_clock::now(); //start time
    //for(int i = 0; i < 100; i++){
    //int iteration = 0;
    //srand(time(NULL));
    //particle = {1,1};                           //particle position
    //particleTmp = particle;                     //save the particle position

    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
    //maze1 = maze.getMaze();
#pragma omp parallel private(numThreads, threadID, direction, particleTmp, maze1, particle, path)
{   
        numThreads = omp_get_num_threads();
        threadID = omp_get_thread_num();
        if(threadID == 0){
            cout << "Number of threads: " << numThreads << endl;
            //cout << "sono il master del mondo" << endl;
        }
        else{
            //cout << "sono il thread " << threadID << endl;
        }
        //cout << "Thread ID: " << threadID << endl;
        //cout << "Thread ID: " << threadID << endl;
        //srand(7634567);
        maze1 = maze.getMaze();	
#pragma omp critical
        maze.printMaze(maze1, 0);                      //print the maze
        //sleep(4);
        //barrier
#pragma omp barrier
        //srand(time(NULL));
        particle = {1,1};                           //particle position
        particleTmp = particle;                     //save the particle position
        //direction = 1;
        while(particle[0] != GRID_DIM - 1 || particle[1] != GRID_DIM - 2){ 
            //cout << "sono nel while " << threadID << endl;
#pragma omp critical
        {
            do{
                direction = rand() % 4;                 //random direction for the particle
                //cout << "sono nel do " << threadID << endl;
                particle = chooseDirection(direction);  //choose the direction for the particle
                //cout << "sono dopo choose direction " << threadID << "  " << direction << endl;
            }while(particleTmp == particle);
        }
            particleTmp = particle;                     //save the particle position 
#pragma omp critical
{
            findPath();                                 //generate the path
}
            cout << "posizione    " << particle[0] << "-" << particle[1] << endl;
            maze1[particle[0]][particle[1]] = 2;        //put the particle in the maze
            //iteration++;
//#pragma omp critical
//{
            //cout << "SONO IL THREAD "<< threadID << endl;
            //maze.printMaze(maze1, 0);                      //print the maze
            //sleep(0.5);
            //usleep(1000000);
//}


        }
}   
        cout << "Solution found" << endl;
        //clear the maze
        /*
        for(int i = 0; i < GRID_DIM; i++){
            for(int j = 0; j < GRID_DIM; j++){
                if(maze1[i][j] == 2){
                    maze1[i][j] = 0;
                }
            }
        }*/

    //auto finish = chrono::high_resolution_clock::now(); //end time

    //chrono::duration<double> elapsed = finish - start;  //elapsed time 
    //elapsed = elapsed / 100;                           //avg elapsed time
    //cout << "Elapsed time: " << elapsed.count() << endl;
}


void Particle::moveRight() {
    if (maze1[particle[0]][particle[1] + 1] == 0 || maze1[particle[0]][particle[1] + 1] == 2) {
        particle[1] = particle[1] + 1;
    }
}

void Particle::moveDown() {
    if (maze1[particle[0] + 1][particle[1]] == 0 || maze1[particle[0] + 1][particle[1]] == 2) {
        particle[0] = particle[0] + 1;
    }
}

void Particle::moveLeft() {
    if (maze1[particle[0]][particle[1] - 1] == 0 || maze1[particle[0]][particle[1] - 1] == 2) {
        particle[1] = particle[1] - 1;
    }
}

void Particle::moveUp() {
    if (particle[0] > 0 && (maze1[particle[0] - 1][particle[1]] == 0 || maze1[particle[0] - 1][particle[1]] == 2)) {
        particle[0] = particle[0] - 1;
    }
}

vector<int> Particle::chooseDirection(int direction) {
    switch (direction) {
        case 0:
            moveRight();
            break;
        case 1:
            moveDown();
            break;
        case 2:
            moveLeft();
            break;
        case 3:
            moveUp();
            break;
    }
    return particle;
}

void Particle::findPath(){                                                                                        //Backtracking the path of the particle insert in path vector the path of the particle
    if(path.size() > 2 && (path[path.size() - 2][0] == particle[0] && path[path.size() - 2][1] == particle[1])){  //if the particle is in the same direction of the previous one
        path.erase(path.end()); 
        path.erase(path.end() - 1);
    } else { 
        path.push_back(particle);
    }

    //bool isPresent = true;        
    
    /*
    isPresent = find(path.begin(), path.end(), particle) != path.end();    //if particle is present in path then isPresent = true
    if(isPresent){
        path.erase(remove(path.begin(), path.end(), particle), path.end()); //if particle is present in path then erase it
        path.push_back(particle); 
    } else {
    path.push_back(particle); 
    }*/                         
}

vector<vector<int>> Particle::getPath(){
    return path;
}



