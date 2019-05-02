#pragma once

#include <mutex>

#include "ProcessDefinition.h"

#include "hermes/FrameReadout.pb.h"

#include <asio/io_service.hpp>


class Frame;
typedef std::shared_ptr<Frame> FramePtr;

class ProcessQueueExecuter  {
public:
	class Overflow : public std::runtime_error {
	public:
		Overflow() : std::runtime_error("Process Overflow") {}
		virtual ~Overflow() {}
	};

	ProcessQueueExecuter(asio::io_service & service, size_t maxWorkers);
	virtual ~ProcessQueueExecuter();

	void Start(ProcessQueue & queue, const Frame::Ptr & current);
	std::string State();

private:
	void SpawnCurrentUnsafe();
	bool IsDoneUnsafe();

	void Init(const ProcessQueue::iterator & begin,
	          const ProcessQueue::iterator & end,
	          const Frame::Ptr & frame);


	asio::io_service &     d_service;
	const size_t           d_maxWorkers;
	std::mutex             d_mutex;
	size_t                 d_nbActiveWorkers;
	ProcessQueue::iterator d_current,d_end,d_nextBegin,d_nextEnd;
	fort::FrameReadout     d_message;
	Frame::Ptr             d_frame,d_next;




	typedef std::vector<cv::Mat> ResultPool;

	ResultPool           d_results;
	ResultPool::iterator d_currentResult;
};