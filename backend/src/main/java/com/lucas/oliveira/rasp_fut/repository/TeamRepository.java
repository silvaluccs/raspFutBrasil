package com.lucas.oliveira.rasp_fut.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.lucas.oliveira.rasp_fut.model.team.Team;
import java.util.List;

@Repository
/**
 * TeamRepository is the repository for the Team entity.
 *
 * @see Team
 * @author Lucas Oliveira
 */
public interface TeamRepository extends JpaRepository<Team, Long> {

  List<Team> findAllByShortName(String shortName);

  List<Team> findAllByName(String name);

}
