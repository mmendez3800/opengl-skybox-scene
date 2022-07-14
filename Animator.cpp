#include "Animator.h"

void Animator::tick(void)
{
	if (started)
	{
		for (std::vector<Animation>::iterator itr = animation_list.begin(); itr != animation_list.end(); ++itr)
		{
			if (itr->viableTime(current_time))
			{
				itr->tick();
			}
		}
		++current_time;
	}
}

void Animator::addAnimation(Animation anim)
{
	animation_list.push_back(anim);
}

void Animator::addInitial(InitialState state)
{
	initial_list.push_back(state);
}

void Animator::resetToInitial(void)
{
	started = false;
	current_time = 0;
	for (std::vector<InitialState>::iterator itr = initial_list.begin(); itr != initial_list.end(); ++itr)
	{
		itr->enact();
	}
}

void Animator::start(void)
{
	started = true;
}

Animation::Animation(long start, long end, Matrix4 modification, Matrix4* modified) : start_time(start), end_time(end), modification(modification), modify(modified)
{
}

Animation::Animation(long start, long end, Matrix4 modification, MatrixTransform* mod_transform) : start_time(start), end_time(end), modification(modification), modify(nullptr), mod_transform(mod_transform)
{
}

void Animation::tick(void)
{
	if (modify)
	{
		*modify = *modify * modification;
	}
	else if (mod_transform)
	{
		mod_transform->postMultMatrix(modification);
	}
	else
	{
		std::cout << "ERROR ANIMATION" << std::endl;
	}
}

bool Animation::viableTime(long time)
{
	return start_time <= time && time < end_time;
}

InitialState::InitialState(Matrix4* modify, Matrix4 state) : matrix(modify), state(state)
{
}

InitialState::InitialState(MatrixTransform* modify, Matrix4 state) : mat_trans(modify), state(state)
{
}

void InitialState::enact(void)
{
	if (matrix)
	{
		*matrix = state;
	}
	else if (mat_trans)
	{
		mat_trans->setMatrix(state);
	}
}

YRotateAnimation::YRotateAnimation(long start, long end, float totalRotate, MatrixTransform* mat_trans) : Animation(start, end, Matrix4(), mat_trans)
{
	modification.makeRotateY(totalRotate / (end - start));
}

TranslateAnimation::TranslateAnimation(long start, long end, Vector3 total_move, MatrixTransform* mat_trans) : Animation(start, end, Matrix4(), mat_trans)
{
	modification.makeTranslate(total_move.scale(1.0f / (end - start)));
}
