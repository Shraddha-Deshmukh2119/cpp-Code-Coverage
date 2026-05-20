package com.ecoomerce.sportscenter.controller;

import com.ecoomerce.sportscenter.model.JwtRequest;
import com.ecoomerce.sportscenter.security.JwtHelper;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@ExtendWith(MockitoExtension.class)
class AuthControllerTest {

    private MockMvc mockMvc;
    private final ObjectMapper objectMapper = new ObjectMapper();

    @Mock
    private UserDetailsService userDetailsService;

    @Mock
    private AuthenticationManager authenticationManager;

    private final JwtHelper jwtHelper = new JwtHelper();

    private AuthConroller authController;

    @BeforeEach
    void setUp() {
        authController = new AuthConroller(userDetailsService, authenticationManager, jwtHelper);
        mockMvc = MockMvcBuilders.standaloneSetup(authController)
                .setControllerAdvice(new BadCredentialsAdvice())
                .build();
    }

    @ControllerAdvice
    static class BadCredentialsAdvice {
        @ExceptionHandler(BadCredentialsException.class)
        public ResponseEntity<Void> handleBadCredentials() {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).build();
        }
    }

    @Test
    void login_returnsToken() throws Exception {
        UserDetails userDetails = User.withUsername("rahul")
                .password("encoded")
                .roles("ADMIN")
                .build();

        when(authenticationManager.authenticate(any(UsernamePasswordAuthenticationToken.class)))
                .thenReturn(null);
        when(userDetailsService.loadUserByUsername("rahul")).thenReturn(userDetails);

        JwtRequest request = new JwtRequest();
        request.setUsername("rahul");
        request.setPassword("Password");

        mockMvc.perform(post("/auth/login")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(objectMapper.writeValueAsString(request)))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.username").value("rahul"))
                .andExpect(jsonPath("$.token").isNotEmpty());
    }

    @Test
    void login_badCredentials_returnsUnauthorized() throws Exception {
        when(authenticationManager.authenticate(any(UsernamePasswordAuthenticationToken.class)))
                .thenThrow(new BadCredentialsException("bad"));

        JwtRequest request = new JwtRequest();
        request.setUsername("rahul");
        request.setPassword("wrong");

        mockMvc.perform(post("/auth/login")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(objectMapper.writeValueAsString(request)))
                .andExpect(status().isUnauthorized());
    }

    @Test
    void getUserDetails_validBearer_returnsUser() throws Exception {
        UserDetails userDetails = User.withUsername("rahul")
                .password("encoded")
                .roles("ADMIN")
                .build();

        String token = jwtHelper.generateToken(userDetails);
        when(userDetailsService.loadUserByUsername("rahul")).thenReturn(userDetails);

        mockMvc.perform(get("/auth/user").header("Authorization", "Bearer " + token))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.username").value("rahul"));
    }

    @Test
    void getUserDetails_missingBearer_returnsBadRequest() throws Exception {
        mockMvc.perform(get("/auth/user").header("Authorization", "Invalid"))
                .andExpect(status().isBadRequest());
    }
}
