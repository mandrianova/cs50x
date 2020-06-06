SELECT title, rating FROM ratings
JOIN movies m on ratings.movie_id = m.id
WHERE year = 2010 and rating IS NOT NULL
ORDER BY rating DESC, title;