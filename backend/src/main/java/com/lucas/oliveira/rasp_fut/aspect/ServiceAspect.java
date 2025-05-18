package com.lucas.oliveira.rasp_fut.aspect;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.stereotype.Component;

import org.springframework.core.annotation.Order;

import java.util.Optional;

import com.lucas.oliveira.rasp_fut.exception.TeamNotFoundException;
import com.lucas.oliveira.rasp_fut.model.team.Team;
import com.lucas.oliveira.rasp_fut.repository.TeamRepository;

@Aspect
@Component
public class ServiceAspect {

  private final TeamRepository teamRepository;

  public ServiceAspect(TeamRepository teamRepository) {
    this.teamRepository = teamRepository;
  }

  @Around(value = "@annotation(com.lucas.oliveira.rasp_fut.annotation.CheckId)")
  @Order(1)
  public Object existingId(ProceedingJoinPoint joinPoint) throws Throwable {

    Object[] args = joinPoint.getArgs();

    Team team = (Team) args[0];

    Long id = team.getId();

    Optional<Team> teamOptional = this.teamRepository.findById(id);

    if (teamOptional.isEmpty()) {

      return joinPoint.proceed();
    }

    throw new TeamNotFoundException("Team with id " + id + " not found.");

  }

  @Around(value = "@annotation(com.lucas.oliveira.rasp_fut.annotation.CheckId)")
  @Order(0)
  public Object validateId(ProceedingJoinPoint joinPoint) throws Throwable {

    Object[] args = joinPoint.getArgs();

    Team team = (Team) args[0];

    Long id = team.getId();

    if (id == null || id <= 0) {
      throw new TeamNotFoundException("Team with id " + id + " is not valid.");
    }

    return joinPoint.proceed();

  }

}
