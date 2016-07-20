//Octave commands: 
//>> f0 = 1000/44100;
//>> b = fir1(10, f0);

//CITE: http://www.eas.uccs.edu/~mwickert/ece2610/lecture_notes/ece2610_chap5.pdf

#define ORDER 11
const float h[ORDER] = {
0.014365,
0.030353,
0.072338,
0.124512,
0.166883,
0.183099,
0.166883,
0.124512,
0.072338,
0.030353,
0.014365
};
float state[ORDER];

//Clears filter history
void init_fir(){
    int i;
    for(i=0; i<ORDER; i++){
        state[i] = 0;
    }
}

//Call for each sample
float iter_fir(float x){
    float out = 0;
    int i;
    //causal fir filter
    state[0] = x;
    for(i=0;i<ORDER;i++){
        out += state[i] * h[i];
    }
    //shift all samples down one
    for(i=(ORDER-1);i>0;i--){
        state[i] = state[i-1];
    }
    return out;
}
