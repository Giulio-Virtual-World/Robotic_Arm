// Mostra la notifica se l'utente è su un dispositivo mobile
window.addEventListener('load', function() {
    const notice = document.getElementById('mobileNotice');
    if (/Mobi|Android|iPhone|iPad|iPod/i.test(navigator.userAgent)) {
        notice.classList.add('show');
        // Nascondi la notifica dopo 5 secondi
        setTimeout(() => {
            notice.classList.add('hide');
            setTimeout(() => notice.style.display = 'none', 500);
        }, 5000);
    }
});

// Smooth scrolling per i link di navigazione
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        
        const targetId = this.getAttribute('href');
        if(targetId === '#') return;
        
        const targetElement = document.querySelector(targetId);
        if(targetElement) {
            window.scrollTo({
                top: targetElement.offsetTop - 100,
                behavior: 'smooth'
            });
        }
    });
});

// Evidenziazione link attivo durante lo scroll
window.addEventListener('scroll', function() {
    const sections = document.querySelectorAll('.section');
    const navLinks = document.querySelectorAll('.nav-links a');
    
    let current = '';
    sections.forEach(section => {
        const sectionTop = section.offsetTop;
        const sectionHeight = section.clientHeight;
        if(scrollY >= (sectionTop - 150)) {
            current = section.getAttribute('id');
        }
    });
    
    navLinks.forEach(link => {
        link.classList.remove('active');
        if(link.getAttribute('href').substring(1) === current) {
            link.classList.add('active');
        }
    });
});