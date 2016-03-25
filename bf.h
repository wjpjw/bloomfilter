#ifndef BF_H_
#define BF_H_

#define MAX_NR_HASH   120
#define MAX_UNIVERSE  1048576

/*
  universe:            {1,2,3,4,...1048576}
  bf length:           n 
  element number:      m
  hash number:         k, k=(ln2)*(n/m)
  input:               m elements in universe
  output:              i in [0,n-1]   
 */

extern bool*                               bf_vector;
extern int                                 n;   // nr vector bits 
extern int                                 m;   // nr elements
extern int                                 k;   // nr hash functions
extern int*                                training_set; // randomly generated, each element distinct
extern int*                                tab;
extern int                                 hash_mask;
  
static inline int                                 hash(int input, int hash_id){
  return (tab[hash_id]+input)%n;
}

static inline void                                bf_create(){
  assert(n>0&&m>0);
  bf_vector=(bool*)malloc(sizeof(bool)*n);
  k=(int)floor(log(2)*(n/m)) +1;  //k=(ln2)*(n/m)
  if(k>MAX_NR_HASH){
    printf("n/m too large!");
    exit(-1);
  }
  tab=(int*)malloc(sizeof(int)*k);
  for(int i=0;i<k;i++){
    tab[i]=rand()/MAX_UNIVERSE;
  }  
}
static inline void                                bf_destroy(){
  free(bf_vector);
}  
static inline void                                bf_add(int input){
  for(int i=0;i<k;i++){
    bf_vector[hash(input,i)]=true;  
  }  
}
static inline bool                                bf_contains(int input){
  for(int i=0;i<k;i++){
    if(!bf_vector[hash(input,i)])return false;  
  }
  return true;
}
static inline bool                                bf_in_training_set(int t,int m){
  for(int i=0;i<m;i++){
    if(training_set[i]==t)return true;
  }
  return false;
}  
//add m elements into bloom filter
static inline void                                bf_train(){
  training_set=(int*)calloc(m,sizeof(int));
  for(int i=0;i<m;i++){
    int t=rand()%MAX_UNIVERSE;  
    while(bf_in_training_set(t,i)){
      t=rand()%MAX_UNIVERSE;  
    }
    training_set[i]=t;
  }
  for(int i=0;i<m;i++){
    bf_add(training_set[i]);
  }  
}
//traverse the universe, return flase positive rate
static inline double                              bf_query_false_positive_rate(){
  int nr_true_negative=0;
  int nr_false_positive=0;
  for(int i=0;i<MAX_UNIVERSE;i++){
    if(bf_contains(i)){
      if(!bf_in_training_set(i,m)){
	nr_false_positive++;
      }
    }else{
      nr_true_negative++;
    }
  }
  assert(nr_false_positive+nr_true_negative>0);
  return (double)(nr_false_positive)/(nr_false_positive+nr_true_negative);
}  
static inline double                              bf_theoretical_false_positive_rate() {
  //return exp((-n/m)*log(2)*log(2));
  double w=1-(double)1/n;
  double t=1-pow(w,k*m);
  return pow(t,k);
}  
#endif
