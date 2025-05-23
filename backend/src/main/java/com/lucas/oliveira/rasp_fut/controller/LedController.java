package com.lucas.oliveira.rasp_fut.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import com.lucas.oliveira.rasp_fut.service.LedService;

@RestController
public class LedController {

  private final LedService ledService;

  public LedController(LedService ledService) {
    this.ledService = ledService;
  }

  @GetMapping("/led/on")
  public String turnOn() {
    String message = ledService.turnOn();
    return message;
  }

  @GetMapping("/led/off")
  public String turnOff() {

    String message = ledService.turnOff();
    return message;

  }

}
