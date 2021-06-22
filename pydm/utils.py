import cv2


def filter_matches(matches, n_top_score):
    return sorted(matches, key=lambda x: x[1], reverse=True)[:n_top_score]


def draw_matches(im1, im2, matches, point_only=False, thickness=1, n_top_score=0):
    # Draw Nth top score match points only
    if n_top_score:
        matches = filter_matches(matches, n_top_score)

    dst = cv2.hconcat([im1, im2])
    for ((x0, y0), (x1, y1)), _ in matches:
        x1 += im1.shape[1]
        if not point_only:
            cv2.line(dst, (x0, y0), (x1, y1), (255, 0, 255), thickness, cv2.LINE_AA)
        cv2.circle(dst, (x0, y0), int(thickness * 1.5), (0, 0, 255), thickness, cv2.LINE_AA)
        cv2.circle(dst, (x1, y1), int(thickness * 1.5), (0, 255, 0), thickness, cv2.LINE_AA)
    return dst
