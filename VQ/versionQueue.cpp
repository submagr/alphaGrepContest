/* Author: Shubham Agrawal
 * Email: agshubh191@gmail.com
 * Details: Contains code for version queue, i.e.( )
 * 1. On each insert, delete, version of queue increases
 * 2. Contents of specific version queue may be asked in query (along with insert, deletes)
 *
 * Implementation:
 * 1. Queue is implemented simply as an array. (PUSH: append at back, POP: move head ptr ahead)
 * 2. Create arrays VersionHead and VersionTail. Keep track of head and tail of every version
 */
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int main(){
    int n;
    scanf("%d", &n);
    int inputBuffer[n+1];
    int versionHead[n+1];
    int versionTail[n+1];
    int currentInputIndex = 1;
    int currentVersionIndex = 1;
    for(int i=0; i<n+1; i++){
        inputBuffer[i] = 0;
        versionHead[i] = 0;
        versionTail[i] = 0;
    }
    versionHead[0] = 1;
    versionTail[0] = 0;

    for(int i=0; i<n; i++){
        char c;
        scanf(" %c", &c);
        if(c=='e'){
            scanf("%d", &inputBuffer[currentInputIndex]);
            currentInputIndex++;
            versionHead[currentVersionIndex] = versionHead[currentVersionIndex-1];
            versionTail[currentVersionIndex] = versionTail[currentVersionIndex-1] + 1;
            currentVersionIndex++;
        }else if(c=='d'){
            if(versionTail[currentVersionIndex] < versionHead[currentVersionIndex]){ // Queue is empty
                versionHead[currentVersionIndex] = versionHead[currentVersionIndex-1];
                versionTail[currentVersionIndex] = versionTail[currentVersionIndex-1];
                currentVersionIndex++;
            }else{
                versionHead[currentVersionIndex] = versionHead[currentVersionIndex-1] + 1;
                versionTail[currentVersionIndex] = versionTail[currentVersionIndex-1];
                currentVersionIndex++;
            }
        }else if(c=='p'){
            int versionToPrint;
            scanf("%d", &versionToPrint);
            printf("printing version %d: ", versionToPrint);
            for(int j=versionHead[versionToPrint]; j<=versionTail[versionToPrint]; j++){
                printf("%d ", inputBuffer[j]);
            }
            printf("\n");
        }
    }
    return 0;
}
