#ifndef KCollide_HEADER_DEFINED
#define KCollide_HEADER_DEFINED

#include "KRect.hpp"

/// \brief A namespace that regroups tools to manage physic collisions.
///
namespace KCollide {

	/// \brief A enumeration of the 4 directions on a screen.
	///
	enum Direction {
		Up, Down, Right, Left
	};

	/// \brief A structure that represents a border.
	///
	/// A border is a physical barrier.
	/// \tparam T the type of the coordinates
	/// \tparam U the tipe of the length
	template <typename T = int, typename U = T>
	struct Border {
		Direction direction;
		T x, y;
		U length;
		int id;
		void assign(Direction direction, T x, T y, U length, int id = 0) {
			this->direction = direction;
			this->x = x;
			this->y = y;
			this->length = length;
			this->id = id;
		}
	};

	/// \brief Detects if the rectangle cross the UP border.
	///
	/// The passed border has to be of type UP.
	/// \param rect the moving rectangle
	/// \param vx the X vector
	/// \param vy the Y vector
	/// \param border the border to test (has to be of type UP)
	/// \return the ratio of cours the rectangle have traveled before touching the border
	template<typename T = int, typename U = T, typename V = int, typename W = V>
	double up(const KRect<T, U>& rect, T vx, T vy, const Border<V, W>& border) {
		if (rect.y >= border.y && rect.y + vy < border.y) {
			float ratio = (border.y - rect.y) / vy;
			vx *= ratio;
			if (rect.x + vx + rect.w > border.x && rect.x + vx < border.x + border.length) {
				if (ratio < 0 || ratio > 1) {
					std::cerr << "UP corrupted ratio : " << ratio << std::endl;
				}
				return ratio;
			}
		}
		return 1.0;
	}
	/// \brief Detects if the rectangle cross the DOWN border.
	///
	/// The passed border has to be of type DOWN.
	/// \param rect the moving rectangle
	/// \param vx the X vector
	/// \param vy the Y vector
	/// \param border the border to test (has to be of type DOWN)
	/// \return the ratio of cours the rectangle have traveled before touching the border
	template<typename T = int, typename U = T, typename V = int, typename W = V>
	double down(const KRect<T, U>& rect, T vx, T vy, const Border<V, W>& border) {
		if (rect.y + rect.h <= border.y && rect.y + rect.h + vy > border.y) {
			float ratio = (border.y - rect.y - rect.h) / vy;
			vx *= ratio;
			if (rect.x + vx + rect.w > border.x && rect.x + vx < border.x + border.length) {
				if (ratio < 0 || ratio > 1) {
					std::cerr << "DOWN corrupted ratio : " << ratio << std::endl;
				}
				return ratio;
			}
		}
		return 1.0;
	}
	/// \brief Detects if the rectangle cross the RIGHT border.
	///
	/// The passed border has to be of type RIGHT.
	/// \param rect the moving rectangle
	/// \param vx the X vector
	/// \param vy the Y vector
	/// \param border the border to test (has to be of type RIGHT)
	/// \return the ratio of cours the rectangle have traveled before touching the border
	template<typename T = int, typename U = T, typename V = int, typename W = V>
	double right(const KRect<T, U>& rect, T vx, T vy, const Border<V, W>& border) {
		if (rect.x + rect.w <= border.x && rect.x + rect.w + vx > border.x) {
			float ratio = (border.x - rect.x - rect.w) / vx;
			vy *= ratio;
			if (rect.y + vy + rect.h > border.y && rect.y + vy < border.y + border.length) {
				if (ratio < 0 || ratio > 1) {
					std::cerr << "RIGHT corrupted ratio : " << ratio << std::endl;
				}
				return ratio;
			}
		}
		return 1.0;
	}
	/// \brief Detects if the rectangle cross the LEFT border.
	///
	/// The passed border has to be of type LEFT.
	/// \param rect the moving rectangle
	/// \param vx the X vector
	/// \param vy the Y vector
	/// \param border the border to test (has to be of type LEFT)
	/// \return the ratio of cours the rectangle have traveled before touching the border
	template<typename T = int, typename U = T, typename V = int, typename W = V>
	double left(const KRect<T, U>& rect, T vx, T vy, const Border<V, W>& border) {
		if (rect.x >= border.x && rect.x + vx < border.x) {
			float ratio = (border.x - rect.x) / vx;
			vy *= ratio;
			if (rect.y + vy + rect.h > border.y && rect.y + vy < border.y + border.length) {
				if (ratio < 0 || ratio > 1) {
					std::cerr << "LEFT corrupted ratio : " << ratio << std::endl;
				}
				return ratio;
			}
		}
		return 1.0;
	}

	/// \brief Find the border in collision with the moving rectangle.
	///
	/// It is going to iterate through a list of border (of any type: UP, DOWN, RIGHT, LEFT)
	/// and find which of these collide the first with the moving rectangle.
	/// If no collision occured, no borders are selected.
	/// \param rect the moving rectangle
	/// \param vx the X vector
	/// \param vy the Y vector
	/// \param start the begining iterator of the list
	/// \param end the ending iterator of the list
	/// \param selected an iterator pointing to the selected border (to end if none)
	/// \return the ratio of cours the rectangle have traveled before touching a border
	template<typename T = int, typename U = T, typename Iter>
	double find(const KRect<T, U>& rect, T vx, T vy, Iter start, Iter end, Iter& selected) {
		double best = 1;
		register double tmp;
		selected = end;
		// for optimization, sort all of 3*3 cases (go up, static or down, times, go left, static or right)
		if (vx > 0) {
			if (vy < 0) {
				// right and up
				while (start != end) {
					if (start->direction == Right) {
						tmp = right(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					} else
					if (start->direction == Up) {
						tmp = up(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			} else
			if (vy > 0) {
				// right and down
				while (start != end) {
					if (start->direction == Right) {
						tmp = right(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					} else
					if (start->direction == Down) {
						tmp = down(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			}
			else {
				// right and static
				while (start != end) {
					if (start->direction == Right) {
						tmp = right(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			}

		} else
		if (vx < 0) {
			if (vy < 0) {
				// left and up
				while (start != end) {
					if (start->direction == Left) {
						tmp = left(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					} else
					if (start->direction == Up) {
						tmp = up(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			} else
			if (vy > 0) {
				// left and down
				while (start != end) {
					if (start->direction == Left) {
						tmp = left(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					} else
					if (start->direction == Down) {
						tmp = down(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			}
			else {
				// left and static
				while (start != end) {
					if (start->direction == Left) {
						tmp = left(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			}
		}
		else {
			if (vy < 0) {
				// static and up
				while (start != end) {
					if (start->direction == Up) {
						tmp = up(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			} else
			if (vy > 0) {
				// static and down
				while (start != end) {
					if (start->direction == Down) {
						tmp = down(rect, vx, vy, *start);
						if (tmp < best) {
							best = tmp;
							selected = start;
						}
					}
					start++;
				}
			}
			// static total have no collision possible
		}
		return best;
	}
}

#endif
