package com.lucas.oliveira.rasp_fut.service.team;

import org.springframework.stereotype.Service;

import com.lucas.oliveira.rasp_fut.repository.TeamRepository;
import com.lucas.oliveira.rasp_fut.annotation.CheckId;
import com.lucas.oliveira.rasp_fut.exception.TeamNotFoundException;
import com.lucas.oliveira.rasp_fut.model.team.Team;

import java.util.List;
import java.util.Optional;
import java.util.logging.Logger;

/**
 * TeamService is the service for the Team entity.
 *
 * @see Team
 * @see TeamRepository
 * @author Lucas Oliveira
 */
@Service
public class TeamService {

  private final TeamRepository teamRepository;

  private Logger logger = Logger.getLogger(TeamService.class.getName());

  /**
   * TeamService constructor.
   *
   * @param teamRepository the team repository
   */
  public TeamService(TeamRepository teamRepository) {
    this.teamRepository = teamRepository;
  }

  /**
   * Saves a team in the database.
   *
   * @param team the team to save
   * @return the saved team
   */
  @CheckId
  public Team save(Team team) {

    logger.info("Saving team: " + team.toString());
    return this.teamRepository.save(team);
  }

  /**
   * Finds all teams in the database.
   *
   * @return all teams in the database
   */
  public List<Team> findAll() {

    return this.teamRepository.findAll();

  }

  /**
   * Finds a team by its id.
   *
   * @param id the id of the team to find
   * @return the team with the given id
   */
  public Optional<Team> findById(Long id) {

    return this.teamRepository.findById(id);

  }

  public void delete(Long id) throws TeamNotFoundException {

    Team team = teamRepository.findById(id)
        .orElseThrow(() -> new TeamNotFoundException("Team with id " + id + " not found."));

    teamRepository.delete(team);

  }

}
