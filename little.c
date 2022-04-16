/**
 * Projec : gtsp (voyageur de commerce)
 *
 * Date   : 07/04/2014
 * Author : Olivier Grunder
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NBR_TOWNS 6

/* Distance matrix */
double dist[NBR_TOWNS][NBR_TOWNS] ;

/* Each edge has a starting and ending node */
int starting_town[NBR_TOWNS] ;
int ending_town[NBR_TOWNS] ;

/* no comment */
int best_solution[NBR_TOWNS] ;
double best_eval=-1.0 ;


/**
 * Berlin52 :
 *  6 towns : Best solution (2315.15): 0 1 2 3 5 4
 * 10 towns : Best solution (2826.50): 0 1 6 2 7 8 9 3 5 4
 */
float coord[NBR_TOWNS][2]=
{
    {565.0,  575.0},
    { 25.0,  185.0},
    {345.0,  750.0},
    {945.0,  685.0},
    {845.0,  655.0},
    {880.0,  660.0}
} ;



/**
 * print a matrix
 */
void print_matrix(double d[NBR_TOWNS][NBR_TOWNS])
{
    int i, j ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("%d:", i) ;
        for (j=0; j<NBR_TOWNS; j++)
        {
            printf ("%6.1f ", d[i][j]) ;
        }
        printf ("\n") ;
    }
}



/**
 * print a solution
 */
void print_solution(int* sol, double eval)
{
    int i ;
    printf ("(%.2f): ", eval) ;
    for (i=0; i<NBR_TOWNS; i++)
        printf ("%d ",sol[i]);
    printf("\n") ;
}




/**
 * evaluation of a solution
 */
double evaluation_solution(int* sol)
{
    double eval=0 ;
    int i ;
    for (i=0; i<NBR_TOWNS-1; i++)
    {
        eval += dist[sol[i]][sol[i+1]] ;
    }
    eval += dist[sol[NBR_TOWNS-1]][sol[0]] ;

    return eval ;

}




/**
 * nearest neighbour solution
 */
double build_nearest_neighbour()
{
    /* solution of the nearest neighbour */
    int sol[NBR_TOWNS] ;

    /* evaluation of the solution */
    double eval = 0 ;

    sol[0] = 0 ;
    int lowest_distance = 99999;
    int closer_town=0;
    int check_old;
    for (int town = 0; town < NBR_TOWNS-1; town++){
        lowest_distance = 99999;
        for (int i = 0; i < NBR_TOWNS; i++){
            check_old=1;
            if (sol[0]!=i && dist[sol[town]][i]<lowest_distance){
                for (int old_town = 0; old_town <= NBR_TOWNS; old_town++){
                    if (sol[old_town]==i){
                        check_old=0;
                    }
                }
                if(check_old==1){ 
                    lowest_distance=dist[sol[town]][i];
                    closer_town=i;
                }
            }
        }
        sol[town+1]=closer_town;
        eval+=lowest_distance;
    }
    eval = evaluation_solution(sol) ;
    printf("Nearest neighbour ") ;
    print_solution (sol, eval) ;
    /*for (i=0;i<NBR_TOWNS;i++) best_solution[i] = sol[i] ;
    best_eval  = eval ;*/
     

    return eval ;
}




/**
 *  Build final solution
 */
void build_solution()
{
    int i, solution[NBR_TOWNS] ;

    int indiceCour = 0;
    int villeCour = 0;

    while (indiceCour < NBR_TOWNS)
    {

        solution[indiceCour] = villeCour ;

        // Test si le cycle est hamiltonien
        for (i = 0; i < indiceCour; i++)
        {
            if (solution[i] == villeCour)
            {
                /* printf ("cycle non hamiltonien\n") ; */
                return;
            }
        }
        // Recherche de la ville suivante
        int trouve = 0;
        int i = 0;
        while ((!trouve) && (i < NBR_TOWNS))
        {
            if (starting_town[i] == villeCour)
            {
                trouve = 1;
                villeCour = ending_town[i];
            }
            i++;
        }
        indiceCour++;
    }

    double eval = evaluation_solution(solution) ;

    if (best_eval<0 || eval < best_eval)
    {
        best_eval = eval ;
        for (i=0; i<NBR_TOWNS; i++)
            best_solution[i] = solution[i] ;
        printf ("New best solution: ") ;
        print_solution (solution, best_eval) ;
    }
    return;
}




/**
 *  Little Algorithm
 */
void little_algorithm(double d0[NBR_TOWNS][NBR_TOWNS], int iteration, double eval_node_parent)
{

    if (iteration == NBR_TOWNS){
        build_solution ();
        return;
    }

    /* Do the modification on a copy of the distance matrix */
    double d[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d, d0, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    int i, j;
    double local_min;

    double eval_node_child = eval_node_parent;

    // add a zero to each line
    for (i = 0; i < NBR_TOWNS; i++){
        local_min=-1.0;
        for ( j = 0; j < NBR_TOWNS; j++){
            if ((d[i][j] < local_min && d[i][j] >=0) ||local_min < 0){
                local_min = d[i][j];
            }
        }

        eval_node_child += local_min;
        for ( j = 0; j < NBR_TOWNS; j++){
            if (d[i][j]>0){
                d[i][j]-=local_min;
            }
        }
    }

    // add a zero to each col
    for (i = 0; i < NBR_TOWNS; i++){
        local_min=-1.0;
        for ( j = 0; j < NBR_TOWNS; j++){
            if ((d[j][i] < local_min && d[i][j] >=0) || local_min < 0){
                local_min = d[j][i];
            }
        }
        
        eval_node_child += local_min;
        for ( j = 0; j < NBR_TOWNS; j++){
            if (d[j][i]>0){
                d[j][i]-=local_min;
            }
        }
    }
    printf("%f\n",eval_node_child);
    print_matrix(d);
    getchar();



    int lineMaxPen, colMaxPen;
    double lineMin, colMin;
    double maxPen = -1.0;
    for ( i = 0; i < NBR_TOWNS;i++){
        for ( j = 0; j < NBR_TOWNS; j++){
            if (d[i][j]==0.0){
            
                lineMin=-1.0;
                for (int k = 0; k < NBR_TOWNS; k++){
                    if ((d[i][k] < lineMin && d[i][k] >0) ||lineMin < 0){
                        lineMin = d[i][k];
                    }
                }
                
                colMin=-1.0;
                for (int k = 0; k < NBR_TOWNS; k++){
                    if ((d[k][j] < colMin && d[k][j] >0) ||colMin < 0){
                        colMin = d[k][j];
                    }
                }
                if (maxPen<colMin+lineMin || maxPen<0){
                    maxPen = colMin+lineMin;
                    lineMaxPen=i;
                    colMaxPen=j;
                } 
            }  
        }  
    }
    if (maxPen==-1.0){
        printf("Solution infeasible\n");
    }else{
        starting_town[iteration] = colMaxPen;
        ending_town[iteration] = lineMaxPen;
        printf("pen:%f,x:%d,y:%d\n",maxPen,lineMaxPen,colMaxPen);
        getchar();
    }
    
    /* Cut : stop the exploration of this node */
    if (best_eval>=0 && eval_node_child >= best_eval)
        return;


    /**
     *  Compute the penalities to identify the zero with max penalty
     *  If no zero in the matrix, then return, solution infeasible
     *  TO COMPLETE
     *  ...
     *  ...
     */
    /* row and column of the zero with the max penalty */
    int izero=-1, jzero=-1 ;

    /**
     *  Store the row and column of the zero with max penalty in
     *  starting_town and ending_town
     *  TO COMPLETE
     *  ...
     *  ...
     */

    /* Do the modification on a copy of the distance matrix */
    double d2[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    /**
     *  Modify the matrix d2 according to the choice of the zero with the max penalty
     *  TO COMPLETE
     *  ...
     *  ...
     */

    /* Explore left child node according to given choice */
    little_algorithm(d2, iteration + 1, eval_node_child);

    /* Do the modification on a copy of the distance matrix */
    memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    /**
     *  Modify the dist matrix to explore the other possibility : the non-choice
     *  of the zero with the max penalty
     *  TO COMPLETE
     *  ...
     *  ...
     */

    /* Explore right child node according to non-choice */
    little_algorithm(d2, iteration, eval_node_child);

}




/**
 *
 */
int main (int argc, char* argv[])
{

    best_eval = -1 ;

    /* Print problem informations */
    printf ("Points coordinates:\n") ;
    int i ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("Node %d: x=%f, y=%f\n", i, coord[i][0], coord[i][1]) ;
    }
    printf ("\n") ;


    for (int x=0; x<NBR_TOWNS; x++){
        for (int y=0; y<NBR_TOWNS; y++){
            if (x==y){
                dist[x][y]=-1;
            }else{
                dist[x][y]=sqrt(pow(coord[x][0]-coord[y][0],2)+pow(coord[x][1]-coord[y][1],2));
            }  
        }
    }

    printf ("Distance Matrix:\n") ;
    print_matrix (dist) ;
    printf ("\n") ;

    double nearest_neighbour = build_nearest_neighbour() ;

    
    int iteration = 0 ;
    double lowerbound = 0.0 ;
    
    little_algorithm(dist, iteration, lowerbound) ;
    
    printf("Best solution:") ;
    print_solution (best_solution, best_eval) ;
    

    printf ("Hit RETURN!\n") ;
    getchar() ;

    return 0 ;
}