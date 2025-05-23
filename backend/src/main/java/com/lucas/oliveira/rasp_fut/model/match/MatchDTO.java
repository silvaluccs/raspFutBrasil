package com.lucas.oliveira.rasp_fut.model.match;

public record MatchDTO(
    Long id,
    String homeTeam,
    String awayTeam,
    String status,
    String time,
    Integer homeScore,
    Integer awayScore

) {

}
