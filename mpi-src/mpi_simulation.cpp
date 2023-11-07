#include <mpi.h>
#include "rc.h"

using namespace std;

int main(int argc, char* argv[])
{

    int N = stoi(argv[1]);
    int steps = stoi(argv[2]);

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Status status;

    int max = size - 1;
    int min = 0;
    int range = max - min + 1;

    RC repcl = RC(rank, N);

    for(int i = 0; i < steps; i++)
    {
        // Choose a random process to send to
        int dest = rand() % range + min;

        int* message = new int[4];
        message[0] = repcl.GetHLC();
        message[1] = repcl.GetBitmap().to_ulong();
        message[2] = repcl.GetOffsets().to_ulong();
        message[3] = repcl.GetCounters();

        MPI_Request request=MPI_REQUEST_NULL;

        MPI_Isend(&message, 4, MPI_INT64_T, dest, 0, MPI_COMM_WORLD, &request);

        dest = 0;

        for(int i = 0; i < size; i++)
        {   
            if(MPI_Iprobe(rank, MPI_ANY_TAG, MPI_COMM_WORLD, &dest, &status) == MPI_SUCCESS) {
                int* m_recv = new int[4];
                MPI_Irecv(&m_recv, 4, MPI_INT64_T, rank, MPI_ANY_TAG, MPI_COMM_WORLD, &request);

                RC m_rc = RC(rank, N);
                m_rc.SetHLC(m_recv[0]);
                m_rc.SetOffsetBitmap(m_recv[1]);
                m_rc.SetOffsets(m_recv[2]);
                m_rc.SetCounters(m_recv[3]);

                auto current_time = std::chrono::system_clock::now();
    
                // Convert the time_point to a duration since the epoch
                auto duration_since_epoch = current_time.time_since_epoch();
                
                // Cast the duration to an integer (in seconds or any other desired unit)
                int current_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_since_epoch).count();

                
                repcl.Recv(m_rc, current_time_seconds);

                if(rank == 0)
                {
                    repcl.PrintClock();
                }
            }
        }

    }

    MPI_Finalize();

    return 0;
}