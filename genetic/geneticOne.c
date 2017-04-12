#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#include<string.h>
#define M_PI 3.14159265358979323846

struct node{

char string[32];
double fitness_value;

};

double fitness_value(char *s){

        int i;double result=0.0,fit_val;
        
        for(i=0;i<15;i++){
        
              double r1 = (double)(s[i])-'0';
              double exp = 5-(i+1);
              
              double r2 = pow(2,exp)*r1;
              result = result + r2;                           
        }
        
        
        
        if(result >=0 && result <= 2*M_PI)
                  fit_val = sin(result)+6;
        
        else if(result > 2*M_PI && result <= 4*M_PI)
                  fit_val = 2*sin(result)+6;
        
        else if(result > 4*M_PI && result <= 6*M_PI)
                  fit_val = 3*sin(result)+6;
        
        else if(result > 6*M_PI && result <= 8*M_PI)
                  fit_val = 4*sin(result)+6;
                  
        else if(result > 8*M_PI && result <= 10*M_PI)
                  fit_val = 5*sin(result)+6;        
        
        else if(result > 10*M_PI && result <= 32)
                  fit_val = sin(result)+6;
                                                      
        

return fit_val;
} 


//crossover function:

void crossover(struct node *a, int l,int index){

        for(;l<15;l++){
                
                char ch = a[index].string[l];
                a[index].string[l] = a[index+1].string[l]; 
                a[index+1].string[l] = ch; 
        }
	a[index].string[l] = '\0'; 
	a[index+1].string[l] = '\0';
}

//Get Mutation probability


double get_mutation_prob(int counter, int N){

        double dy = (0.49-0.15);double dx,m;
        
        if(counter <= N/3){
        
                dx = N/3;
                m = dy/dx;
                return (m*counter);
        }
        
        else if(counter > N/3 && counter <= ((2*N)/3)){
        
                dx = N/3;
                return((1-((counter - N/3)/dx))*dy);
        }
        
        else{
        
                dx = N/3;
                m = dy/dx;
                return(m*(counter-((2*N)/3)));
        }

}



int main(){

    FILE *fp,*fp2;
    int l,counter=0;
    int m;
    int i=0;
	struct node sample[10];
	struct node *all_strings;
    time_t t;
	struct node sample1[10],sample_temp[10],sample2[10],sample3[10],sample4[10];
	int diff_sample_count,same_sample_count;
	int N;
        
    printf("Please input number of iterations :");
    scanf("%d",&N);
        
        
    fp = fopen("populationString.txt","r");
    fp2 = fopen("geneticOut.txt","w");
        
        
        
    all_strings = (struct node *)malloc(32768*sizeof(struct node));
        
        
    while(i<32768){
    	fread(all_strings[i].string, 1, 16, fp);                                
        i++;
    }
       
	i=0;        
    while(i<32768){
    	all_strings[i].fitness_value = fitness_value(all_strings[i].string);
        i++;
    }
        
    srand((unsigned) time(&t));
	double avg = 0.0;
        
	fprintf(fp2,"Running for 10 different initial populations\n\n\n");

	for(diff_sample_count=0;diff_sample_count<10;diff_sample_count++){ // start of different sample count 
		avg = 0.0;        
		for(i=0;i<10;i++){
           	int r = rand()%32768;
           	int j;
           	for(j=0;j<15;j++){
            	sample[i].string[j] = all_strings[r].string[j];
                sample4[i].string[j] = all_strings[r].string[j];
            }
            sample[i].string[j]='\0';
            sample4[i].string[j]='\0';
            sample[i].fitness_value = all_strings[r].fitness_value;
            sample4[i].fitness_value = all_strings[r].fitness_value;
        }   
  
	 	fprintf(fp2,"Initial population is \n\n");       
 		fprintf(fp2,"--------------------------------------------------------------------------\n");
 		for(i=0;i<10;i++){
        	fprintf(fp2,"%s %lf\n",sample[i].string,sample[i].fitness_value);
	 	}
        fprintf(fp2,"\nResult based on above initial population is \n\n");       
 	 	for(same_sample_count=0;same_sample_count<10;same_sample_count++){  // start of same sample count
			counter=0;
		 	while(counter < N){
        		double max=0.0;int index;struct node s0;
        		int p;
        
       			for(p=0;p<10;p++){
       	        	if(sample[p].fitness_value > max){
               	        max = sample[p].fitness_value;
               	        index = p;
       	        	}                         
       			} 
        		for(p=0;p<15;p++){
        	       	s0.string[p] = sample[index].string[p];
        		}
        		s0.string[p] = '\0';
        		s0.fitness_value = max;        
        		//procedure for the selection of Q1:
        
        
        
       			for(i=1;i<10;i++){                                      //qumulative fitness value of Q0 
           			sample[i].fitness_value = sample[i-1].fitness_value + sample[i].fitness_value;
   				} 
        
       			double total = sample[9].fitness_value;        
       			for(i=0;i<10;i++){                                      //selection with prob proportionate fitness value
           			sample[i].fitness_value = sample[i].fitness_value/total;
       			}  
        		for(i=0;i<10;i++){                                      //selection with prob proportionate fitness value
                	double d = (double)((rand()%32768))/32768.0;
                	//printf("*%lf*",d);
                	int j;
                	for(j=0;j<10;j++){        
                		if(d <= sample[j].fitness_value)  
                     		break; 
                	}
                	int k;               
                	for(k=0;k<15;k++)
                		sample1[i].string[k] = sample[j].string[k];
                	sample1[i].string[k]='\0';
                	sample1[i].fitness_value = fitness_value(sample1[i].string);;                       
        		}
        
                               
        		//Procedure to get Q2 and crossover****************************** 
        
        		for(i=0;i<10;i=i+2){
        			l = rand()%15;
           			//printf("%d\n",l);
           			if(((double)(rand()%32768)/32768.0)<=0.85){
        	   			crossover(sample1,l,i);
               			sample1[i].fitness_value = fitness_value(sample1[i].string);
						sample1[i+1].fitness_value = fitness_value(sample1[i+1].string);
					}
                                                                                  
                }
        
        
        		//Procedure to get Q3 i.e after mutation******************************
        
        		double p_mut,p_mut1; 
        		for(i=0;i<10;i++){
                	int j;
                	for(j=0;j<15;j++){
                		p_mut1 = get_mutation_prob(counter,N);
                		p_mut = (double)((rand()%32768)/32768.0);
                		//printf("*%lf*",p_mut);
                		if(p_mut<=p_mut1){
                			if(sample1[i].string[j] == '0') sample1[i].string[j]='1';
                	    	else sample1[i].string[j]='0';
                  		}
               			else
               				sample1[i].string[j] = sample1[i].string[j]; 
                	}
                	sample1[i].string[j] = '\0';
                	sample1[i].fitness_value = fitness_value(sample1[i].string);
                } 
         
               
        		//maximum in Q3 is s1:
        
        		struct node s1;
        		max = 0.0;
               
        		for(p=0;p<10;p++){
                	if(sample1[p].fitness_value > max){
                		max = sample1[p].fitness_value;
                   			index = p;
                	}                         
        		}
        
        
        		for(p=0;p<15;p++){
        	    	s1.string[p] = sample1[index].string[p];
        		}
        		s1.string[p] = '\0';
        		s1.fitness_value = max;
        
        
        		//if s0.ft_val > s1.fit_val then remove a string randomly from Q3 and add s0:
        
		        if(s0.fitness_value > s1.fitness_value){
               		int r = rand()%10;
               		int j;
               		for(j=0;j<15;j++){
                    	sample1[r].string[j] = s0.string[j];
                    	sample1[r].fitness_value = s0.fitness_value;
                    }               
        		}
        
        		for(i=0;i<10;i++){//Now copy the content of Q3 to Q0
                	int j;
                	for(j=0;j<15;j++){
                		sample[i].string[j] = sample1[i].string[j];
                	}
                	sample[i].string[j]='\0';
                	sample[i].fitness_value = sample1[i].fitness_value;
               	}  
        
        
        		counter++;      
			} // end of the while loop
  
  
  
 			//Print the result:
       		double max=0.0;int index; struct node s0;
       		int p;
       		for(p=0;p<10;p++){
            	if(sample[p].fitness_value > max){
            		max = sample[p].fitness_value;
                    index = p;
                }                         
        	}
        
        
        	for(p=0;p<15;p++){
               	s0.string[p] = sample[index].string[p];
        	}
        	s0.string[p] = '\0';
        
        	s0.fitness_value = max;
        	fprintf(fp2,"GA Run %d and Fitness output: %s %lf \n",same_sample_count+1,s0.string,s0.fitness_value);
			avg += s0.fitness_value;
                                 
        	//fprintf(fp2,"\n");
        
        
        	for(i=0;i<10;i++){
        		int j;
        	    for(j=0;j<15;j++){
        		    sample[i].string[j] = sample4[i].string[j];
               	}	
               	sample[i].string[j]='\0';
               	sample[i].fitness_value = sample4[i].fitness_value;
            }   
		}// end of same sample count  
  
 	fprintf(fp2,"Average value for this population is: %lf", avg/10); 
 	fprintf(fp2,"\n--------------------------------------------------------------------------\n\n");    
 	} // end of different sample count 
  
 fclose(fp);
 fclose(fp2);

 return 0;
               
} // end of main
