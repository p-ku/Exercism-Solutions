SELECT COUNT(title) FROM movies
JOIN stars
ON stars.movie_id = movies.id
JOIN ratings
ON ratings.movie_id = movies.id
JOIN people
ON stars.person_id = people.id
WHERE name = "Chadwick Boseman"
ORDER BY rating desc
LIMIT 5;