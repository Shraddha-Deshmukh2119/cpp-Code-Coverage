package com.ecoomerce.sportscenter.controller;

import com.ecoomerce.sportscenter.model.BasketItemResponse;
import com.ecoomerce.sportscenter.model.BasketResponse;
import com.ecoomerce.sportscenter.service.BasketService;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

import java.util.List;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.delete;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@ExtendWith(MockitoExtension.class)
class BasketControllerTest {

    private MockMvc mockMvc;
    private final ObjectMapper objectMapper = new ObjectMapper();

    @Mock
    private BasketService basketService;

    @InjectMocks
    private BasketController basketController;

    @BeforeEach
    void setUp() {
        mockMvc = MockMvcBuilders.standaloneSetup(basketController).build();
    }

    @Test
    void getAllBaskets_returnsList() throws Exception {
        when(basketService.getAllBaskets()).thenReturn(List.of(
                BasketResponse.builder().id("b1").items(List.of()).build()));

        mockMvc.perform(get("/api/baskets"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].id").value("b1"));
    }

    @Test
    void getBasketById_returnsBasket() throws Exception {
        when(basketService.getBasketById("b1")).thenReturn(
                BasketResponse.builder().id("b1").items(List.of()).build());

        mockMvc.perform(get("/api/baskets/b1"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value("b1"));
    }

    @Test
    void deleteBasketById_callsService() throws Exception {
        mockMvc.perform(delete("/api/baskets/b1"))
                .andExpect(status().isOk());

        verify(basketService).deleteBasketById("b1");
    }

    @Test
    void createBasket_returnsCreated() throws Exception {
        BasketResponse request = BasketResponse.builder()
                .id("b2")
                .items(List.of(BasketItemResponse.builder()
                        .id(1)
                        .name("Ball")
                        .description("desc")
                        .price(100L)
                        .pictureUrl("url")
                        .productBrand("Nike")
                        .productType("Shoes")
                        .quantity(1)
                        .build()))
                .build();

        when(basketService.createBasket(any())).thenReturn(request);

        mockMvc.perform(post("/api/baskets")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(objectMapper.writeValueAsString(request)))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.id").value("b2"));
    }
}
