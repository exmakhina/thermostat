#ifndef __MSGQUEUE_H__
#define __MSGQUEUE_H__

#include <queue>
#include <mutex>

using namespace std;

template <class T>
class MsgQueue
{
public:
	MsgQueue();
	~MsgQueue();
	
	int receive(T&);	/* blocking */
	int poll(T&);		/* non-blocking */
	int send(T);
	
private:
	mutex mtx;
	queue<T> msq;
};

/**** Implementation ****/

template <class T>
MsgQueue<T>::MsgQueue()
{
	mtx.lock();
	while (!msq.empty()) msq.pop();
	mtx.unlock();
}

template <class T>
MsgQueue<T>::~MsgQueue()
{
	mtx.lock();
	while (!msq.empty()) msq.pop();
	mtx.unlock();
}

template <class T>
int MsgQueue<T>::receive(T &msg)
{
	while (msq.empty());	// wait until msq is not empty
	
	mtx.lock();
	msg = msq.front();
	msq.pop();
	mtx.unlock();
	
	return 0;
}

template <class T>
int MsgQueue<T>::poll(T &msg)
{
	if (!msq.empty()) {
		mtx.lock();
		msg = msq.front();
		msq.pop();
		mtx.unlock();
		return 0;
	} else {
		return -1;
	}
}

template <class T>
int MsgQueue<T>::send(T msg)
{
	mtx.lock();
	msq.push(msg);
	mtx.unlock();
	
	return 0;
}

#endif /* __MSGQUEUE_H__ */
