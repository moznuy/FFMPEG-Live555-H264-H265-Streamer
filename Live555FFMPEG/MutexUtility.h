#ifndef MUTEXUTILITY_H__
#define MUTEXUTILITY_H__

//#include <Windows.h>

//Helper to specify Timeout
inline bool LOCK( std::timed_mutex& aMutex, int aTimeOut )
{
	//Timeout
//	if( WaitForSingleObject( aMutex, aTimeOut /*Do not change this variable*/ ) == WAIT_OBJECT_0 ) {
	return aMutex.try_lock_for(std::chrono::milliseconds(aTimeOut));
}

//Helper to lock a mutex
inline bool LOCK( std::timed_mutex& aMutex)
{
	//Fixed timeout of 0
	return LOCK( aMutex, 10 );
}

//Unlock mutex
inline bool UNLOCK( std::timed_mutex& aMutex )
{
	aMutex.unlock();
//	return ReleaseSemaphore( aMutex ,1,NULL)?1:0;
}
#endif // MUTEXUTILITY_H__