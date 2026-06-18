
void foo(int x);
void bar(int x);

int main(void){
    foo(10);
    bar(15);
    return 0;
}

void foo(int x){
    int y;
    y = x +5;
}

void bar(int x){
    int k, m;
    k = x + m;
    foo(12);
}